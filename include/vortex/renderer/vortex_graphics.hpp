#pragma once

namespace vortex::renderer
{
	struct VxColor
	{
		unsigned char r, g, b, a;
	};

	struct VxTexture
	{
		unsigned int id;
		int width, height;
		int mipmaps;
		int format;
	};
}