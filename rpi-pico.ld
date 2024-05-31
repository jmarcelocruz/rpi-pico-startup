/*
 * Copyright 2024 José Marcelo Marques da Cruz
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

ENTRY(_reset)

MEMORY {
    stage2 (r)   : ORIGIN = 0x10000000, LENGTH = 256
    flash  (rx)  : ORIGIN = 0x10000000 + LENGTH(stage2), LENGTH = 2048k - LENGTH(stage2)
    sram   (rwx) : ORIGIN = 0x20000000, LENGTH = 264k - LENGTH(stage2)
    sram_stage2 (x) : ORIGIN = ORIGIN(sram) + LENGTH(sram), LENGTH = LENGTH(stage2)

}

_estack = ORIGIN(sram) + LENGTH(sram);

SECTIONS {
    .stage2 : ALIGN(4) {
        *(.stage2)
        . = LENGTH(stage2) - 4;
        LONG(0xDEADBEEF)
    } > sram_stage2 AT> stage2
    .text : ALIGN(4) {
        *(.vectors)
        *(.text)
        *(.text.*)
    } > flash
    .rodata : ALIGN(4) {
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
        _erodata = .;
    } > flash
    .data : ALIGN(4) {
        _sdata = .;
        *(.data)
        *(.data.*)
        . = ALIGN(4);
        _edata = .;
    } > sram AT> flash
    .bss : ALIGN(4) {
        _sbss = .;
        *(.bss)
        *(.bss.*)
        . = ALIGN(4);
        _ebss = .;
    } > sram
    _sheap = .;
}
