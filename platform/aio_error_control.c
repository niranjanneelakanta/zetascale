//----------------------------------------------------------------------------
// ZetaScale
// Copyright (c) 2016, SanDisk Corp. and/or all its affiliates.
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published by the Free
// Software Foundation;
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License v2.1 for more details.
//
// A copy of the GNU Lesser General Public License v2.1 is provided with this package and
// can also be found at: http://opensource.org/licenses/LGPL-2.1
// You should have received a copy of the GNU Lesser General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place, Suite 330, Boston, MA 02111-1307 USA.
//----------------------------------------------------------------------------

/*
 * File:   $URL: svn://svn.schoonerinfotech.net/schooner-trunk/trunk/sdf/platform/aio_error_control.c $
 * Author: drew
 *
 * Created on May 21, 2010
 *
 * (c) Copyright 2010, Schooner Information Technology, Inc.
 * http://www.schoonerinfotech.com/
 *
 * $Id: aio_error_control.c 13945 2010-06-02 01:01:15Z drew $
 */

#include "platform/aio_error_control.h"
#include "platform/aio_error_control_internal.h"
#include "platform/errno.h"
#include "platform/logging.h"
#include "platform/string.h"

#define LOG_CAT PLAT_LOG_CAT_PLATFORM_AIO

void
paio_error_control_config_init(struct paio_error_control_config *config) {
#ifdef notyet
    /* XXX: drew 2010-05-27 Coverity doesn't like the zero sized copy */
    memset(config, 0, sizeof (*config));
#endif
}

void
paio_error_control_config_destroy(struct paio_error_control_config *config) {
}

void
paio_error_control_config_dup(struct paio_error_control_config *dest,
                              const struct paio_error_control_config *src) {
    *dest = *src;
}

int
paio_error_control_set_error(struct paio_error_control *error_control,
                             enum paio_ec_type error_type,
                             const char *filename, off_t start, off_t len) {
    return ((*error_control->set_error)(error_control, error_type, filename,
                                        start, len));
}

const char *
paio_ec_type_to_string(enum paio_ec_type error) {
    switch (error) {
#define item(caps, value, lower) case caps: return (#lower);
    PAIO_EC_TYPE_ITEMS()
#undef item
    }

    return ("Invalid");
}

int
paio_parse_ec_type(enum paio_ec_type *out, const char *arg) {
    int ret;

#define item(caps, value, lower) \
    if (strcmp(arg, #lower) == 0) {                                            \
        *out = caps;                                                           \
        ret = 0;                                                               \
    } else
    PAIO_EC_TYPE_ITEMS()
#undef item
    {
        ret = -EINVAL;
    }

    return (ret);
}

const char *
paio_parse_ec_type_usage() {
    static const char ret[] =
#define item(caps, value, lower) #lower " "
    PAIO_EC_TYPE_ITEMS();
#undef item

    return (ret);
}
