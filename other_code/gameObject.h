	#pragma once

	#include <glm/glm.hpp>

	#include <string>
	#include <vector>

	class Material;

	class GameObject
	{
		private:
			//Inner classes
			class IndexedVertex
			{
				public:
					int pos;
					int texture;
					int normal;
			};

			struct ObjectGroup
			{
				std::string name;
				int materialIndex = -1;
				std::vector<std::vector<IndexedVertex>> faces;
			};

			//Vars
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> textureCoords;
			std::vector<ObjectGroup*> groups;

			//Material loader
			void LoadMaterialFile(const std::string& mtlPath);

		protected:
			std::vector<Material*> materials;
			glm::vec3 transform = glm::vec3(0, 0, 0);

		public:
			GameObject() = default;
			explicit GameObject(glm::vec3 trans);
			GameObject(const std::string& dir, const std::string& fileName);
			~GameObject(void);

			void Draw() const;

			//Object loader
			void LoadObjectFile(const std::string& dir, const std::string& fileName);

			//String extension methods
			std::string Replace(std::string& str, const std::string& toReplace, const std::string& replacement);
			std::vector<std::string> Split(std::string str, const std::string& separator);
			inline std::string ToLower(std::string data);
			inline std::string CleanLine(std::string line);
	};
