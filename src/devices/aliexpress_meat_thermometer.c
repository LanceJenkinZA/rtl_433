/** @file
    Generic AliExpress Meat Thermometer.

    Copyright (C) 2022 Lance Jenkin

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/
/**
Generic AliExpress Meat Thermometer.

Data format:

    [00] {32} E5 18 60 46
              II TT UU tC

              II - ID
              TT - Temperature (F)
              UU - Unknown
              t - Temperature Upper Nibble
              C - Checksum
*/

#include "decoder.h"

static int aliexpress_meat_thermometer_decode(r_device *decoder, bitbuffer_t *bitbuffer)
{
    uint8_t *b;
    data_t *data;

    // All 9 rows contain the same information. Return on first decoded row.

    // A row has 32 bits
    if (bitbuffer->bits_per_row[0] != 32) {
        return DECODE_ABORT_LENGTH;
    }
    b = bitbuffer->bb[0];

    int id       = (b[0]);
    int temp_raw = ((b[3] & 0x40) << 2) | b[1];
    float temp_F = (temp_raw - 90);
    float temp_C = (temp_F - 32) * 5 / 9;

    /* clang-format off */
    data = data_make(
            "model",             "",       DATA_STRING, "AliExpressMeatThermometer",
            "id",                "ID",                   DATA_INT,    id,
            "temperature_F",     "Temperature",          DATA_COND,   temp_raw != 0, DATA_FORMAT, "%.01f F", DATA_DOUBLE, temp_F,
            "temperature_C",     "Temperature",          DATA_COND,   temp_raw != 0, DATA_FORMAT, "%.01f C", DATA_DOUBLE, temp_C,
            NULL);
    /* clang-format on */

    decoder_output_data(decoder, data);
    return 1;
}

static char *output_fields[] = {
        "model",
        "id",
        "temperature_F",
        "temperature_C",
        NULL,
};

r_device alimeatthermo = {
        .name        = "AliExpress Meat Thermometer",
        .modulation  = OOK_PULSE_PPM,
        .short_width = 2012,
        .long_width  = 4008,
        .gap_limit   = 4032,
        .reset_limit = 9008,
        .decode_fn   = &aliexpress_meat_thermometer_decode,
        .fields      = output_fields,
};
