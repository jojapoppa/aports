FROM alpine as builder

ENV ROOT_URL https://eve-alpine-packages.s3.amazonaws.com/edge

WORKDIR /rootfs
ADD ${ROOT_URL}/images/alpine-minirootfs-210509-riscv64.tar.gz /tmp/
RUN tar xzf /tmp/*tar.gz
ADD ${ROOT_URL}/keys/builder@projecteve.dev-608ede5d.rsa.pub etc/apk/keys/
RUN printf "${ROOT_URL}/main\n${ROOT_URL}/community\n" > etc/apk/repositories

FROM scratch

# seed the root filesystem
COPY --from=builder /rootfs/ /

# construct the builder environment
ENV APORTS /home/builder/aports
RUN apk add abuild curl tar make linux-headers patch g++ git gcc ncurses-dev autoconf file
RUN adduser -G abuild -D builder
RUN su builder -c 'git config --global user.email "builder@projecteve.dev" && git config --global user.name "Project EVE"'
RUN su builder -c 'abuild-keygen -a -n'
RUN su builder -c 'mkdir /home/builder/packages'
RUN cp /home/builder/.abuild/*.pub /etc/apk/keys
RUN chmod 664 /etc/apk/keys/*

# hook aports up to rvs for now
RUN su builder -c 'git clone --depth 1 https://github.com/rvs/aports $APORTS'

# set the defaults for docker run
ENV PATH /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
USER builder
WORKDIR /home/builder
CMD sh
