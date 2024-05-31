# Copyright 2024 José Marcelo Marques da Cruz
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

VERSION_MAJOR = 1
VERSION_MINOR = 1
VERSION_PATCH = 0

CFLAGS = -mcpu=cortex-m0plus -mthumb -ffreestanding -Iexternal/rpi-pico-hardware-regs-1.1.0/include ${EXTRA_CFLAGS}

SOURCES = startup.c syscalls.c
OBJS = $(patsubst %.c,%.o,${SOURCES})

dist: rpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.tar.gz
rpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.tar.gz: librpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.a rpi-pico.ld LICENSE
	mkdir -p rpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}/licenses/
	ln librpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.a rpi-pico.ld rpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}/
	ln LICENSE rpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}/licenses/
	tar -cv rpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}/ | gzip > $@
	rm -r rpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}/

librpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.a: ${OBJS}
	${AR} rcs $@ $^

${OBJS}: %.o: ${SOURCES}
	${CC} $^ -c ${CFLAGS}

.PHONY: clean
clean:
	rm -f ${OBJS} rpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.a rpi-pico-startup-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.tar.gz
