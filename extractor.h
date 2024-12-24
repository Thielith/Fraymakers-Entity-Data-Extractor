#ifndef ENTITY_DATA_EXTRACTOR_EXTRACTOR_H
#define ENTITY_DATA_EXTRACTOR_EXTRACTOR_H

#include "stat-tracker.h"
#include "struct-definitions.h"
#include "external/jsoncpp.cpp"

class entityDataExtractor {
	public:
		entityData* extractEntityData(std::string);
		void printStats();
		void printStats(std::string);
		
	private:
		statTracker tracker;
		enumTranslate translator;
	
		std::list<animation>* getAnimations(Json::Value);
		void printAnimationData(std::list<animation>*);

		std::list<keyframe*>* getKeyframes(Json::Value);
		keyframe* extractKeyframeByType(Json::Value);
		void printKeyframeData(std::list<keyframe*>*);
		void printKeyframeTypeData(keyframe*);

		std::list<layer*>* getLayers(Json::Value);
		layer* extractLayerByType(Json::Value);
		void printLayerData(std::list<layer*>*);
		void printLayerTypeData(layer*);
		paletteMap* getPaletteMap(Json::Value);
		void printPaletteMapData(paletteMap*);

		std::list<symbol*>* getSymbols(Json::Value);
		symbol* extractSymbolByType(Json::Value);
		void printSymbolData(std::list<symbol*>*);
		void printSymbolTypeData(symbol*);

		void printTitle(std::string);
};

#endif