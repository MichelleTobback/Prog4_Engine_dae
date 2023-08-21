#pragma once

#include "Scene/Serializable.h"
#include <glm/glm.hpp>

namespace dae
{
	class Component;
	struct Vec2
	{
		Vec2() = default;
		Vec2(const glm::vec2& vec)
			: x{ vec.x }, y{ vec.y } {}
		float x{}, y{};

		operator glm::vec2() const { return glm::vec2{ x, y }; }
	};
	struct Vec3
	{
		Vec3() = default;
		Vec3(const glm::vec3& vec)
			: x{ vec.x }, y{ vec.y }, z{ vec.z } {}
		float x{}, y{}, z{};

		operator glm::vec3() const { return glm::vec3{ x, y, z }; }
	};
	struct Vec4
	{
		Vec4() = default;
		Vec4(const glm::vec4& vec)
			: x{ vec.x }, y{ vec.y }, z{ vec.z }, w{ vec.w } {}
		float x{}, y{}, z{}, w{};

		operator glm::vec4() const { return glm::vec4{ x, y, z, w }; }
	};

	//=================================
	// Quad
	//=================================
	class QuadSerializer final : public Serializable
	{
	public:
		QuadSerializer();
		virtual ~QuadSerializer() = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// textureRenderer
	//=================================
	class TextureRendererSerializer final : public Serializable
	{
	public:
		TextureRendererSerializer();
		virtual ~TextureRendererSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// SpriteRenderer
	//=================================
	class SpriteRendererSerializer final : public Serializable
	{
	public:
		SpriteRendererSerializer();
		virtual ~SpriteRendererSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// QuadRenderer
	//=================================
	class QuadRendererSerializer final : public Serializable
	{
	public:
		QuadRendererSerializer();
		virtual ~QuadRendererSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// Texture
	//=================================
	class TextureSerializer final : public Serializable
	{
	public:
		TextureSerializer();
		virtual ~TextureSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// TextComponent
	//=================================
	class TextComponentSerializer final : public Serializable
	{
	public:
		TextComponentSerializer();
		virtual ~TextComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// SpriteComponent
	//=================================
	class SpriteComponentSerializer final : public Serializable
	{
	public:
		SpriteComponentSerializer();
		virtual ~SpriteComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// SpriteAtlasComponent
	//=================================
	class SpriteAtlasComponentSerializer final : public Serializable
	{
	public:
		SpriteAtlasComponentSerializer();
		virtual ~SpriteAtlasComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// RigidBody2DComponent
	//=================================
	class RigidBody2DComponentSerializer final : public Serializable
	{
	public:
		RigidBody2DComponentSerializer();
		virtual ~RigidBody2DComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// BoxCollider2DComponent
	//=================================
	class BoxCollider2DComponentSerializer final : public Serializable
	{
	public:
		BoxCollider2DComponentSerializer();
		virtual ~BoxCollider2DComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};

	//=================================
	// Tag
	//=================================
	class TagComponentSerializer final : public Serializable
	{
	public:
		TagComponentSerializer();
		virtual ~TagComponentSerializer() override = default;

		virtual void Serialize(BinaryWriter& out, Component* pComponent) const override;
		virtual Component* Deserialize(DeserializeParams& params) override;

	private:
	};
}