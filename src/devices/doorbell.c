/** @file
    Door Bell

    Copyright (C) 2022 Lance Jenkin

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
*/
/**
Generic AliExpress Meat Thermometer.

Data format:

    [00] {13} EA A8
              II II

              II - Device ID, always EA A8
*/

#include "decoder.h"

static int doorbell_decode(r_device *decoder, bitbuffer_t *bitbuffer)
{
    uint8_t *b;
    data_t *data;

    // check if any row contains the ID
    for (int i = 0; i < bitbuffer->num_rows; ++i) {
        b = bitbuffer->bb[i];
        if (b[0] == 0xEA && b[1] == 0xA8) {
            break;
        }
        if (i == bitbuffer->num_rows - 1) {
            return DECODE_ABORT_EARLY;
        }
    }


    /* clang-format off */
    data = data_make(
            "model",             "",       DATA_STRING, "Doorbell",
            "door_ring",                "Ring?",                   DATA_INT,    1,
            NULL);
    /* clang-format on */

    decoder_output_data(decoder, data);
    return 1;
}

static char *output_fields[] = {
        "model",
        "door_ring",
        NULL,
};

r_device doorbell = {
        .name        = "Door Bell",
        .modulation  = OOK_PULSE_PWM,
        .short_width = 284,
        .long_width  = 660,
        .gap_limit   = 792,
        .reset_limit = 6072,
        .decode_fn   = &doorbell_decode,
        .fields      = output_fields,
};
