#include "ClassMapping.hpp"
#include <unordered_map>
#include <deque>
#include <sstream>

namespace IL2CPP
{
	namespace ClassMapping
	{
		namespace INTERNAL
		{
			enum class ClassQueueType
			{
				CLASS_NAME,
				PATTERN,
			};

			class ClassQueue
			{
				public:
				ClassQueueType type;
				std::string keyname;

				// FOR PATTERN TYPE
				const FieldPattern* pattern = nullptr;

				ClassQueue(const std::string& className)
				{
					this->type = ClassQueueType::CLASS_NAME;

					this->keyname = className;
				}

				ClassQueue(const std::string& keyname, const FieldPattern* pattern)
				{
					this->type = ClassQueueType::PATTERN;

					this->keyname = keyname;
					this->pattern = pattern;
				}

				std::string ToString()
				{
					std::stringstream out;

					if (this->type == ClassQueueType::CLASS_NAME)
					{
						out << "Type: CLASS_NAME" << std::endl;
						out << "Keyname: " << keyname;
					}
					else if(this->type == ClassQueueType::PATTERN)
					{
						out << "Type: PATTERN" << std::endl;
						out << "Keyname: " << keyname << std::endl;
						out << "Pattern: {";

						for (size_t i = 0; i < pattern->size(); i++)
						{
							const char* v = pattern->at(i);
							v = v == nullptr ? "nullptr" : v;

							if (i != pattern->size() - 1)
							{
								out << "\"" << v << "\"" ", ";
							}
							else
							{
								out << "\"" << v << "\"";
							}
						}

						out << "}";
					}

					return out.str();
				}
			};

			std::unordered_map<std::string, Class*> classMap;
			std::unordered_map<std::string, std::deque<ClassQueue>> queueMap;
			std::vector<Image*> imageToScan;

			void HandleClass(Class* klass)
			{
				const std::string& classNamespace = klass->GetNamespace();
				const std::string& className = klass->GetName();

				auto namespaceIter = queueMap.find(classNamespace);
				if (namespaceIter == queueMap.end())
				{
					return;
				}

				for (auto queueIter = namespaceIter->second.begin(); queueIter != namespaceIter->second.end();)
				{
					bool matches = 
						(queueIter->type == ClassQueueType::CLASS_NAME && className == queueIter->keyname) ||
						(queueIter->type == ClassQueueType::PATTERN && klass->CompareFieldPattern(queueIter->pattern));

					if (matches)
					{
						classMap.emplace(queueIter->keyname, klass);
						queueIter = namespaceIter->second.erase(queueIter);
					}
					else
					{
						++queueIter;
					}
				}

				if (namespaceIter->second.empty())
				{
					queueMap.erase(namespaceIter);
				}
			}
		}

		void AddImageToScan(Image* image)
		{
			if (image == nullptr)
			{
				throw Exception::NullArgument(IL2CPP_NAMEOF(image));
			}

			INTERNAL::imageToScan.push_back(image);
		}

		void AddImageToScan(const std::string& assemblyName)
		{
			Image* image = IL2CPP::CurrentDomain()->OpenAssembly(assemblyName);

			INTERNAL::imageToScan.push_back(image);
		}

		void AddQueue(const std::string& className, const std::string& namespaze)
		{
			INTERNAL::queueMap.emplace(namespaze, std::initializer_list<INTERNAL::ClassQueue>{});
			INTERNAL::queueMap[namespaze].push_back(
				INTERNAL::ClassQueue(
					className
				)
			);
		}

		void AddQueue(const std::string& mapKeyname, const std::string& namespaze, const FieldPattern* pattern)
		{
			if (pattern == nullptr)
			{
				throw Exception::NullArgument(IL2CPP_NAMEOF(pattern));
			}

			INTERNAL::queueMap.emplace(namespaze, std::initializer_list<INTERNAL::ClassQueue>{});
			INTERNAL::queueMap[namespaze].push_back(
				INTERNAL::ClassQueue(
					mapKeyname,
					pattern
				)
			);
		}

		void ClearImagesToScan()
		{
			INTERNAL::imageToScan.clear();
		}

		void ClearMapping()
		{
			INTERNAL::classMap.clear();
		}

		void ClearQueue()
		{
			INTERNAL::queueMap.clear();
		}

		void StartMapping()
		{
			for(Image* image : INTERNAL::imageToScan)
			{
				image->IterateClasses(INTERNAL::HandleClass);
			}

			if (!INTERNAL::queueMap.empty())
			{
				size_t queueSize = 0;
				std::stringstream msg;

				msg << std::endl;

				for (auto& namespaceIter : INTERNAL::queueMap)
				{
					for (INTERNAL::ClassQueue& queue : namespaceIter.second)
					{
						msg << queue.ToString() << std::endl;
						msg << "---------------" << std::endl;

						queueSize++;
					}
				}

				ClearQueue();
				throw Exception::UnresolvedMappingQueue(queueSize, msg.str());
			}

			ClearQueue();
		}

		Class* GetClass(const std::string& mapKeyname)
		{
			auto it = INTERNAL::classMap.find(mapKeyname);

			if (it == INTERNAL::classMap.end() || it->second == nullptr)
			{
				throw std::runtime_error(
					std::format(
						"Couldn't find result with \"{0}\" key.", 
						mapKeyname
					)
				);
				printf("Couldn't find result with \"{0}\" key.");
				return nullptr;
			}

			return it->second;
		}
	}
}