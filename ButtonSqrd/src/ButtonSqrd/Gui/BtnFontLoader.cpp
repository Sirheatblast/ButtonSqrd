#include "BtnFontLoader.h"
namespace BtnSqd {
	std::shared_ptr<Texture> BtnFontLoader::LoadFont(std::string path, std::map<int, BtnGlyph>& btnGlyphs) {
		std::shared_ptr<Texture> fontTexture;

		if (msdfgen::FreetypeHandle* ftHandle = msdfgen::initializeFreetype()) {
			if (msdfgen::FontHandle* font = msdfgen::loadFont(ftHandle, path.c_str())) {
				std::vector<msdf_atlas::GlyphGeometry> glyphs;
				msdf_atlas::FontGeometry fontGeo(&glyphs);
				fontGeo.loadCharset(font, 1.0f, msdf_atlas::Charset::ASCII);
				const double maxCornerAngle = 3.0;
				for (auto& glyph : glyphs) {
					glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);
				}

				int width = 0;
				int height = 0;
				msdf_atlas::TightAtlasPacker packer;

				packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
				packer.setMinimumScale(24.0);
				packer.setPixelRange(3.0);
				packer.setMiterLimit(1.0);
				packer.pack(glyphs.data(), glyphs.size());
				packer.getDimensions(width, height);

				msdf_atlas::ImmediateAtlasGenerator<float,4,msdf_atlas::mtsdfGenerator,
					msdf_atlas::BitmapAtlasStorage<msdfgen::byte, 4>> generator(width, height);

				msdf_atlas::GeneratorAttributes attributes;
				generator.setAttributes(attributes);
				generator.setThreadCount(4);
				generator.generate(glyphs.data(), glyphs.size());

				msdfgen::BitmapConstRef<msdfgen::byte, 4> bitmap = generator.atlasStorage();
				if (bitmap.pixels) {
					TextureSettings texSet;
					texSet.texDataType = TextureDataType::Color;
					texSet.texFormat = TexFormat::RGBA;
					fontTexture.reset(Texture::Create(bitmap.width, bitmap.height, texSet, (void*)bitmap.pixels));
				}

				for (const auto& glyph : glyphs) {
					double al,ab,ar,at;
					double pl, pb, pr, pt;

					glyph.getQuadAtlasBounds(al,ab,ar,at);
					glyph.getQuadPlaneBounds(pl, pb, pr, pt);

					BtnGlyph bGlyph;
					bGlyph.uvAxis = glm::vec4(
						(float)al/width,			
						(float)ab/height,		
						(float)ar/width,		
						(float)at/height					
					);
					
					bGlyph.glyphSize = glm::vec4(
						(float)pl,
						(float)pb,
						(float)pr,
						(float)pt
					);

					bGlyph.advance = (float)glyph.getAdvance();
					btnGlyphs[glyph.getCodepoint()] = bGlyph;
				}
			}

			msdfgen::deinitializeFreetype(ftHandle);
		}

		return fontTexture;
	}
}
