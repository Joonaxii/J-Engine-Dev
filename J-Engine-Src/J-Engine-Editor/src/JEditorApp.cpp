#include <JEngine/Core/EntryPoint.h>
#include <JEngine/Core/GameObject.h>
#include <JEngine/Components/CTransform.h>

namespace JEngine {
	class JEditor : public Application {
	public:
		JEditor(const AppSpecs& specs)
			: Application(specs)
		{
			//TODO: Init
		}

		static void printInfo(const char* header = nullptr, const char* footer = nullptr) {
			if (header) {
				JENGINE_TRACE(header);
			}

			auto& goAlloc = GameObject::getGameObjectAllocator();
			auto mainChGo = goAlloc.getMainChunk();
			JENGINE_TRACE("[PoolAllocator - GameObject] : {0} chunks", goAlloc.getChunkCount());

			for (size_t i = 0; i < goAlloc.getChunkCount(); i++) {
				JENGINE_TRACE("[PoolAllocator - GameObject] : #{0:02X} {1:064b}", i, mainChGo ? mainChGo->getUsageMask() : 0ULL);
				mainChGo = mainChGo->next;
			}


			//auto& trAlloc = Component::getComponentAllocator<CTransform>();
			//auto mainCh = trAlloc.getMainChunk();
			//JENGINE_TRACE("[PoolAllocator - CTransform] : {0} chunks, {1:064b} 1st chunk", trAlloc.getChunkCount(), mainCh ? mainCh->getUsageMask() : 0ULL);

			if (footer) {
				JENGINE_TRACE(footer);
			}
		}

		void run() override {
			auto& renderer = getRenderer();

			printInfo();

			std::vector<GameObject*> gos{};
			GameObject* go = GameObject::createObject("Test");
			auto tr = go->addComponent<CTransform>();

			gos.push_back(go);

			printInfo();
			while (true) {
				bool shouldBreak = false;
				auto& time = getTime();

				if (Input::isDown(Input::INP_F9)) {
					GameObject::getGameObjectAllocator().reserve(1024);
					JENGINE_INFO("Reserved 1024 GameObjects");
					printInfo();
				}


				if (Input::isDown(Input::INP_F1)) {
					for (size_t i = 0; i < 1024; i++) {
						GameObject* goA = GameObject::createObject("TestBB");
						//auto trA = goA->addComponent<CTransform>();
						gos.push_back(goA);
					}
					printInfo();
				}

				if (Input::isDown(Input::INP_F2)) {
					if (gos.size()) {
						size_t randInd = rand() % gos.size();

						gos[randInd] = GameObject::destroyObject(gos[randInd]);
						gos.erase(gos.begin() + randInd);

						//JENGINE_INFO("Destroyed Object at index {0}", randInd);
						//printInfo();
					}
				}

				if (Input::isDown(Input::INP_F3)) {
					GameObject::getGameObjectAllocator().trim();
					//JENGINE_INFO("Trimmed GameObject Pool");
					//printInfo();
				}

				if (Input::isDown(Input::INP_F4)) {
					GameObject::getGameObjectAllocator().clear(false);
					gos.clear();
					//JENGINE_INFO("Cleared GameObject Pool");
					//printInfo();
				}


				if (Input::isDown(Input::INP_F5)) {
					GameObject::getGameObjectAllocator().clear(true);
					gos.clear();
					JENGINE_INFO("Cleared GameObject Pool (Full)");
					//printInfo();
				}



				if (Input::isDown(Input::INP_F6)) {
					size_t maxx = std::min<size_t>(32, gos.size());
					for (int i = maxx - 1; i >= 0; i--) {
						gos[i] = GameObject::destroyObject(gos[i]);
						gos.erase(gos.begin() + i);
					}

					JENGINE_INFO("Removed '{0}' GameObject Pool", maxx);
					printInfo();
				}

				shouldBreak |= !renderer.doRender();
				if (shouldBreak) { break; }
				time.tick();
				Input::update(true);
			}
		}
	private:

	};

	Application* createApplication(AppArgs args) {
		AppSpecs specs;

		char temp[513]{ 0 };
		sprintf_s(temp, "J-Engine V%s [%s]", JENGINE_VERSION.toString().c_str(), JENGINE_VERSION_STR);

		specs.name = std::string(temp);
		specs.args = args;
		return new JEditor(specs);
	}
}