/*-
 * Copyright (c) 2015 Allan Jude <allanjude@FreeBSD.org>
 * All rights reserved.
 *
 * This software was developed by Edward Tomasz Napierala under sponsorship
 * from the FreeBSD Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libnvpair.h>
#include <sys/vdev_impl.h>

#include "fstyp.h"

int
fstyp_zfs(FILE *fp, char *label, size_t labelsize)
{
	vdev_label_t *zpool_ptr = NULL;
	vdev_label_t zpool_label;
	char *buf = zpool_label.vl_vdev_phys.vp_nvlist;
	char *zpool_name = NULL;
	size_t buflen = sizeof (zpool_label.vl_vdev_phys.vp_nvlist);
	nvlist_t *config = NULL;

	zpool_ptr = (vdev_label_t *)read_buf(fp, 0, sizeof(zpool_label));
	if (zpool_ptr == NULL)
		return (1);
	zpool_label = *zpool_ptr;
	if (nvlist_unpack(buf, buflen, &config, 0) != 0)
		goto zfserr;
	if (nvlist_lookup_string(config, "name", &zpool_name) != 0)
		goto zfserr;
	strlcpy(label, zpool_name, labelsize);
	nvlist_free(config);
	free(zpool_ptr);
	return (0);

zfserr:
	nvlist_free(config);
	free(zpool_ptr);

	return (1);
}
