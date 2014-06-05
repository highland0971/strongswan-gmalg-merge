/*
 * Copyright (C) 2014 Martin Willi
 * Copyright (C) 2014 revosec AG
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "cng_plugin.h"
#include "cng_hasher.h"

#include <library.h>

typedef struct private_cng_plugin_t private_cng_plugin_t;

/**
 * private data of cng_plugin
 */
struct private_cng_plugin_t {

	/**
	 * public functions
	 */
	cng_plugin_t public;
};

METHOD(plugin_t, get_name, char*,
	private_cng_plugin_t *this)
{
	return "cng";
}

METHOD(plugin_t, get_features, int,
	private_cng_plugin_t *this, plugin_feature_t *features[])
{
	static plugin_feature_t f[] = {
		PLUGIN_REGISTER(HASHER, cng_hasher_create),
			PLUGIN_PROVIDE(HASHER, HASH_MD2),
			PLUGIN_PROVIDE(HASHER, HASH_MD4),
			PLUGIN_PROVIDE(HASHER, HASH_MD5),
			PLUGIN_PROVIDE(HASHER, HASH_SHA1),
			PLUGIN_PROVIDE(HASHER, HASH_SHA256),
			PLUGIN_PROVIDE(HASHER, HASH_SHA384),
			PLUGIN_PROVIDE(HASHER, HASH_SHA512),
	};
	*features = f;
	return countof(f);
}

METHOD(plugin_t, destroy, void,
	private_cng_plugin_t *this)
{
	cng_hasher_deinit();
	free(this);
}

/*
 * see header file
 */
plugin_t *cng_plugin_create()
{
	private_cng_plugin_t *this;

	INIT(this,
		.public = {
			.plugin = {
				.get_name = _get_name,
				.get_features = _get_features,
				.destroy = _destroy,
			},
		},
	);

	cng_hasher_init();

	return &this->public.plugin;
}
