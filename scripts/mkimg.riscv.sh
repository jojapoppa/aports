# [abuild-]apk add abuild apk-tools alpine-conf busybox fakeroot xorriso mtools squashfs-tools mkinitfs
# ln -s python3.9 /usr/bin/python3
# 
# ./mkimage.sh [--hostkeys] --outdir /tmp/out --profile riscv --repository http://eve-alpine-packages.s3.amazonaws.com/edge/main --extra-repository http://eve-alpine-packages.s3.amazonaws.com/edge/community

profile_riscv() {
	profile_base
	title="Generic RISCV64"
	desc="Has default RISCV64 kernel.
		Includes the uboot bootloader.
		Supports armv7 and aarch64."
	image_ext="tar.gz"
	arch="riscv64"
	kernel_flavors="virt"
	hostname="alpine"
	uboot_install="yes"
}
