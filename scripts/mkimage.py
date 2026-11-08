#!/usr/bin/env python3
"""Package an x86-64 UEFI application in a qcow2 disk image."""

import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile

SECTOR_SIZE = 512
PARTITION_START = 2048
PARTITION_SIZE = 60 * 1024 * 1024
DISK_SIZE = 64 * 1024 * 1024


def run(*args):
    subprocess.run(args, check=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('efi', type=Path, help='x86-64 UEFI application')
    parser.add_argument('output', type=Path, help='qcow2 image path')
    parser.add_argument('--kernel', required=True, type=Path, help='kernel ELF file')
    args = parser.parse_args()

    if not args.efi.is_file():
        parser.error(f'UEFI application not found: {args.efi}')

    if not args.kernel.is_file():
        parser.error(f'kernel not found: {args.kernel}')

    if args.kernel.resolve() == args.output.resolve():
        parser.error('kernel and output must be different files')

    if args.efi.resolve() == args.output.resolve():
        parser.error('input and output must be different files')

    if not args.output.parent.is_dir():
        parser.error(f'output directory does not exist: {args.output.parent}')

    for tool in ('sgdisk', 'mkfs.fat', 'mmd', 'mcopy', 'qemu-img'):
        if shutil.which(tool) is None:
            parser.error(f'required tool not found: {tool}')

    with tempfile.TemporaryDirectory(prefix='.pintos-image-', dir=args.output.parent) as tmp:
        raw = Path(tmp) / 'disk.raw'
        fat = Path(tmp) / 'esp.fat'
        qcow = Path(tmp) / 'disk.qcow2'

        with raw.open('wb') as disk:
            disk.truncate(DISK_SIZE)

        with fat.open('wb') as partition:
            partition.truncate(PARTITION_SIZE)

        end = PARTITION_START + PARTITION_SIZE // SECTOR_SIZE - 1

        run('sgdisk', '--clear', f'--new=1:{PARTITION_START}:{end}',
            '--typecode=1:ef00', '--change-name=1:EFI System', str(raw))
        run('mkfs.fat', '-F', '32', str(fat))
        run('mmd', '-i', str(fat), '::/EFI', '::/EFI/BOOT')
        run('mcopy', '-i', str(fat), str(args.efi), '::/EFI/BOOT/BOOTX64.EFI')
        run('mcopy', '-i', str(fat), str(args.kernel), '::/kernel.elf')

        with raw.open('r+b') as disk, fat.open('rb') as partition:
            disk.seek(PARTITION_START * SECTOR_SIZE)
            shutil.copyfileobj(partition, disk)

        run('qemu-img', 'convert', '-f', 'raw', '-O', 'qcow2', str(raw), str(qcow))

        os.replace(qcow, args.output)

    print(f'Created {args.output}')


if __name__ == '__main__':
    try:
        main()
    except (OSError, subprocess.CalledProcessError) as error:
        print(f'mkimage: {error}', file=sys.stderr)
        sys.exit(1)
