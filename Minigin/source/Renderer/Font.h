#pragma once
#include <string>

struct _TTF_Font;
namespace dae
{
	/**
	 * Simple RAII wrapper for a _TTF_Font
	 */
	class Font final
	{
	public:
		_TTF_Font* GetFont() const;
		explicit Font(const std::string& path, const std::string& fileName, unsigned int size);
		~Font();

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;

		inline unsigned int GetSize() const { return m_size; }
		inline const std::string& GetPath() const { return m_Path; }

	private:
		_TTF_Font* m_font;
		unsigned int m_size;
		std::string m_Path{};
	};
}
