#include "extractor.h"
#include "struct-definitions.h"

// tracker:
// track how many plugins are used

entityData* entityDataExtractor::extractEntityData(std::string path){
	std::ifstream entityFile(path, std::ifstream::binary);
	Json::Value data;
	entityFile >> data;

	entityData* extractedData = new entityData;
	extractedData->animations = getAnimations(data);
	extractedData->shouldExport = data["export"].asBool();
	extractedData->guid = data["guid"].asString();
	extractedData->id = data["id"].asString();
	extractedData->keyframes = getKeyframes(data);
	extractedData->layers = getLayers(data);
	extractedData->paletteMap = getPaletteMap(data);
	extractedData->plugins = getPlugins(data);
	extractedData->pluginMetadata = getPluginMetadata(data, extractedData->plugins);
	extractedData->symbols = getSymbols(data);
	extractedData->tags = getTags(data);
	extractedData->version = data["version"].asUInt();
	
	return extractedData;
}

void entityDataExtractor::printStats(){
	tracker.printStats();
}
void entityDataExtractor::printStats(std::string outputPath){
	tracker.printStats(outputPath);
}

std::list<animation>* entityDataExtractor::getAnimations(Json::Value data){
	const std::string sectionName = "animations";
	std::list<animation> *extractedAnimations = new std::list<animation>;

	unsigned int dataIndex = 0;
	while(data[sectionName][dataIndex]){
		Json::Value dataSnippit = data[sectionName][dataIndex];
		animation entry;
		
		entry.id = dataSnippit["$id"].asString();
		entry.name = dataSnippit["name"].asString();

		unsigned int layerIndex = 0;
		while(dataSnippit["layers"][layerIndex]){
			entry.layerIDs.push_back(dataSnippit["layers"][layerIndex].asString());
			layerIndex++;
		}

		extractedAnimations->push_back(entry);
		tracker.incrementAnimations();

		dataIndex++;
	}

	return extractedAnimations;
}
void entityDataExtractor::printData(std::list<animation>* data){
	printTitle("Animations");
	for(animation entry : *data){
		std::cout << entry.name << std::endl;
		std::cout << "\tid: " << entry.id << std::endl;
		
		std::cout << "\tlayer IDs:\n";
		for(std::string layerID : entry.layerIDs){
			std::cout << "\t\t" << layerID << std::endl;
		}
	}
}

std::list<keyframe*>* entityDataExtractor::getKeyframes(Json::Value data){
	const std::string sectionName = "keyframes";
	std::list<keyframe*> *extractedKeyframes = new std::list<keyframe*>;

	unsigned int dataIndex = 0;
	while(data[sectionName][dataIndex]){
		Json::Value dataSnippit = data[sectionName][dataIndex];
		keyframe* entry = extractKeyframeByType(dataSnippit);
		
		entry->id = dataSnippit["$id"].asString();
		entry->length = dataSnippit["length"].asUInt();

		extractedKeyframes->push_back(entry);
		tracker.incrementKeyframes();

		dataIndex++;
	}

	return extractedKeyframes;
}
keyframe* entityDataExtractor::extractKeyframeByType(Json::Value keyframeData){
	KEYFRAME_LAYER_TYPE type = translator.toKeyframeLayerEnum(keyframeData["type"].asString());
	
	if(type == KL_FRAME_SCRIPT){
		tracker.incrementScriptKeyframes();

		keyframeScript* keyframe = new keyframeScript;
		keyframe->code = keyframeData["code"].asString();
		return keyframe;
	}
	else if(
		type == KL_IMAGE ||
		type == KL_COLLISION_BOX ||
		type == KL_COLLISION_BODY || 
		type == KL_POINT || 
		type == KL_LINE_SEGMENT
	){
		if(type == KL_IMAGE){ tracker.incrementImageKeyframes(); }
		else if(type == KL_COLLISION_BOX){ tracker.incrementCollisionBoxKeyframes(); }
		else if(type == KL_COLLISION_BODY){ tracker.incrementCollisionBodyKeyframes(); }
		else if(type == KL_POINT){ tracker.incrementPointKeyframes(); }
		else if(type == KL_LINE_SEGMENT){ tracker.incrementLineSegmentKeyframes(); }

		keyframeAnimated* keyframe = new keyframeAnimated(type);
		keyframe->symbolID = keyframeData["symbol"].asString();
		keyframe->tweenType = translator.toTweenEnum(keyframeData["tweenType"].asString());
		keyframe->tweened = keyframeData["tweened"].asBool();

		if(!keyframe->tweened){
			return keyframe;
		}
		if(keyframe->tweenType == TW_LINEAR){ tracker.incrementLinearTweens(); }
		else if(keyframe->tweenType == TW_EASE_IN_QUAD){ tracker.incrementEaseInTweens(); tracker.incrementQuadTweens(); }
		else if(keyframe->tweenType == TW_EASE_OUT_QUAD){ tracker.incrementEaseOutTweens(); tracker.incrementQuadTweens(); }
		else if(keyframe->tweenType == TW_EASE_IN_OUT_QUAD){ tracker.incrementEaseInOutTweens(); tracker.incrementQuadTweens(); }
		else if(keyframe->tweenType == TW_EASE_IN_CUBIC){ tracker.incrementEaseInTweens(); tracker.incrementCubicTweens(); }
		else if(keyframe->tweenType == TW_EASE_OUT_CUBIC){ tracker.incrementEaseOutTweens(); tracker.incrementCubicTweens(); }
		else if(keyframe->tweenType == TW_EASE_IN_OUT_CUBIC){ tracker.incrementEaseInOutTweens(); tracker.incrementCubicTweens(); }
		else if(keyframe->tweenType == TW_EASE_IN_QUART){ tracker.incrementEaseInTweens(); tracker.incrementQuartTweens(); }
		else if(keyframe->tweenType == TW_EASE_OUT_QUART){ tracker.incrementEaseOutTweens(); tracker.incrementQuartTweens(); }
		else if(keyframe->tweenType == TW_EASE_IN_OUT_QUART){ tracker.incrementEaseInOutTweens(); tracker.incrementQuartTweens(); }
		else if(keyframe->tweenType == TW_EASE_IN_QUINT){ tracker.incrementEaseInTweens(); tracker.incrementQuintTweens(); }
		else if(keyframe->tweenType == TW_EASE_OUT_QUINT){ tracker.incrementEaseOutTweens(); tracker.incrementQuintTweens(); }
		else if(keyframe->tweenType == TW_EASE_IN_OUT_QUINT){ tracker.incrementEaseInOutTweens(); tracker.incrementQuintTweens(); }

		return keyframe;
	}
	else if(type == KL_LABEL){
		tracker.incrementLabelKeyframes();

		keyframeLabel* keyframe = new keyframeLabel;
		keyframe->name = keyframeData["name"].asString();
		return keyframe;
	}
	else if(type == KL_CONTAINER){
		tracker.incrementContainerKeyframes();

		keyframe* keyframe = new ::keyframe;
		keyframe->type = KL_CONTAINER;
		return keyframe;
	}

	return nullptr;
}
void entityDataExtractor::printData(std::list<keyframe*>* data){
	printTitle("Keyframes");
	for(keyframe* entry : *data){
		std::cout << "\tid: " << entry->id << std::endl;
		std::cout << "\tlength: " << entry->length << std::endl;
		std::cout << "\ttype: " << translator.toString(entry->type) << std::endl;

		printData(entry);

		std::cout << std::endl;
	}
}
void entityDataExtractor::printData(keyframe* data){
	if(keyframeScript* converted = dynamic_cast<keyframeScript*>(data)){
		std::cout << "\tcode: \"" << converted->code << "\"" << std::endl;
	}
	else if(keyframeAnimated* converted = dynamic_cast<keyframeAnimated*>(data)){
		std::cout << "\tsymbolID: \"" << converted->symbolID << "\"" << std::endl;
		std::cout << "\ttweenType: " << converted->tweenType << std::endl;
		std::cout << "\ttweened: " << converted->tweened << std::endl;
	}
	else if(keyframeLabel* converted = dynamic_cast<keyframeLabel*>(data)){
		std::cout << "\tname: \"" << converted->name << "\"" << std::endl;
	}
}

std::list<layer*>* entityDataExtractor::getLayers(Json::Value data){
	const std::string sectionName = "layers";
	std::list<layer*> *extractedLayers = new std::list<layer*>;

	unsigned int dataIndex = 0;
	while(data[sectionName][dataIndex]){
		Json::Value dataSnippit = data[sectionName][dataIndex];
		layer* entry = extractLayerByType(dataSnippit);
		
		entry->id = dataSnippit["$id"].asString();
		entry->hidden = dataSnippit["hidden"].asBool();
		entry->locked = dataSnippit["locked"].asBool();
		entry->name = dataSnippit["name"].asString();
		
		unsigned int keyframeIndex = 0;
		while(dataSnippit["keyframes"][keyframeIndex]){
			entry->keyframeIDs.push_back(dataSnippit["keyframes"][keyframeIndex].asString());
			keyframeIndex++;
		}

		extractedLayers->push_back(entry);
		tracker.incrementLayers();

		dataIndex++;
	}

	return extractedLayers;
}
layer* entityDataExtractor::extractLayerByType(Json::Value layerData){
	KEYFRAME_LAYER_TYPE type = translator.toKeyframeLayerEnum(layerData["type"].asString());
	
	if(type == KL_FRAME_SCRIPT){
		tracker.incrementScriptLayers();

		layerScript* layer = new layerScript;
		layer->language = layerData["language"].asString();
		return layer;
	}
	else if(type == KL_COLLISION_BOX){
		tracker.incrementCollisionBoxLayers();

		layerCollisionBox* layer = new layerCollisionBox;
		layer->defaultAlpha = layerData["defaultAlpha"].asFloat();
		layer->defaultColor = layerData["defaultColor"].asString();
		
		// layer->collisionBoxType = 
		if(layerData["pluginMetadataEntry"].compare("{}")){
			Json::Value snippit = layerData["pluginMetadataEntry"]["com.fraymakers.FraymakersMetadata"];
			layer->collisionBoxType = translator.toCollisionBoxEnum(snippit["collisionBoxType"].asString());
			layer->collisionBoxIndex = snippit["index"].asInt();

			if(layer->collisionBoxType == NONE){ tracker.incrementNoneBoxes(); }
			else if(layer->collisionBoxType == HURT_BOX){ tracker.incrementHurtBoxes(); }
			else if(layer->collisionBoxType == HIT_BOX){ tracker.incrementHitBoxes(); }
			else if(layer->collisionBoxType == GRAB_BOX){ tracker.incrementGrabBoxes(); }
			else if(layer->collisionBoxType == LEDGE_GRAB_BOX){ tracker.incrementLedgeGrabBoxes(); }
			else if(layer->collisionBoxType == REFLECT_BOX){ tracker.incrementReflectBoxes(); }
			else if(layer->collisionBoxType == ABSORB_BOX){ tracker.incrementAbsorbBoxes(); }
			else if(layer->collisionBoxType == COUNTER_BOX){ tracker.incrementCounterBoxes(); }
			else if(layer->collisionBoxType == CUSTOM_BOX_A){ tracker.incrementCustomBoxAs(); }
			else if(layer->collisionBoxType == CUSTOM_BOX_B){ tracker.incrementCustomBoxBs(); }
			else if(layer->collisionBoxType == CUSTOM_BOX_C){ tracker.incrementCustomBoxCs(); }
		}
		return layer;
	}
	else if(type == KL_COLLISION_BODY){
		tracker.incrementCollisionBodyLayers();

		layerCollisionBody* layer = new layerCollisionBody;
		layer->defaultAlpha = layerData["defaultAlpha"].asFloat();
		layer->defaultColor = layerData["defaultColor"].asString();
		layer->defaultHead = layerData["defaultHead"].asUInt();
		layer->defaultHipWidth = layerData["defaultHipWidth"].asUInt();
		layer->defaultHipXOffset = layerData["defaultHipXOffset"].asUInt();
		layer->defaultHipYOffset = layerData["defaultHipYOffset"].asUInt();
		layer->defaultFoot = layerData["defaultFoot"].asUInt();
		return layer;
	}
	else if(type == KL_IMAGE){
		tracker.incrementImageLayers();

		layer* layer = new ::layer;
		layer->type = KL_IMAGE;
		return layer;
	}
	else if(type == KL_LABEL){
		tracker.incrementLabelLayers();
		
		layer* layer = new ::layer;
		layer->type = KL_LABEL;
		return layer;
	}
	else if(type == KL_POINT){
		tracker.incrementPointLayers();

		layer* layer = new ::layer;
		layer->type = KL_POINT;
		return layer;
	}
	else if(type == KL_LINE_SEGMENT){
		tracker.incrementLineSegmentLayers();

		layer* layer = new ::layer;
		layer->type = KL_LINE_SEGMENT;
		return layer;
	}
	else if(type == KL_CONTAINER){
		tracker.incrementContainerLayers();

		layer* layer = new ::layer;
		layer->type = KL_CONTAINER;
		return layer;
	}

	return nullptr;
}
void entityDataExtractor::printData(std::list<layer*>* data){
	printTitle("Layers");
	for(layer* entry : *data){
		std::cout << "\tname: " << entry->name << std::endl;
		std::cout << "\tid: " << entry->id << std::endl;
		std::cout << "\thidden: " << entry->hidden << std::endl;
		std::cout << "\tlocked: " << entry->locked << std::endl;

		std::cout << "\tkeyframe IDs:\n";
		for(std::string keyframeID : entry->keyframeIDs){
			std::cout << "\t\t" << keyframeID << std::endl;
		}
		std::cout << "\ttype: " << translator.toString(entry->type) << std::endl;

		printData(entry);

		std::cout << std::endl;
	}
}
void entityDataExtractor::printData(layer* data){
	if(layerScript* converted = dynamic_cast<layerScript*>(data)){
		std::cout << "\tlanguage: \"" << converted->language << "\"" << std::endl;
	}
	else if(layerCollisionBox* converted = dynamic_cast<layerCollisionBox*>(data)){
		std::cout << "\tdefaultAlpha: " << converted->defaultAlpha << std::endl;
		std::cout << "\tdefaultColor: " << converted->defaultColor << std::endl;
		std::cout << "\tcollisionBoxType: " << translator.toString(converted->collisionBoxType) << std::endl;
		std::cout << "\tindex: " << converted->collisionBoxIndex << std::endl;

	}
	else if(layerCollisionBody* converted = dynamic_cast<layerCollisionBody*>(data)){
		std::cout << "\tdefaultAlpha: " << converted->defaultAlpha << std::endl;
		std::cout << "\tdefaultColor: " << converted->defaultColor << std::endl;
		std::cout << "\tdefaultHead: " << converted->defaultHead << std::endl;
		std::cout << "\tdefaultHipWidth: " << converted->defaultHipWidth << std::endl;
		std::cout << "\tdefaultHipXOffset: " << converted->defaultHipXOffset << std::endl;
		std::cout << "\tdefaultHipYOffset: " << converted->defaultHipYOffset << std::endl;
		std::cout << "\tdefaultFoot: " << converted->defaultFoot << std::endl;
	}
}

paletteMap* entityDataExtractor::getPaletteMap(Json::Value data){
	const std::string sectionName = "paletteMap";
	paletteMap *extractedPaletteMap = new paletteMap;

	extractedPaletteMap->palletteCollectionID =
		(data[sectionName]["paletteCollection"].asString().empty()) ?
		"null" :
		data[sectionName]["paletteCollection"].asString();
	extractedPaletteMap->paletteMapID =
		(data[sectionName]["paletteMap"].asString().empty()) ?
		"null" :
		data[sectionName]["paletteMap"].asString();

	return extractedPaletteMap;
}
void entityDataExtractor::printData(paletteMap* data){
	printTitle("Palette Map");
	std::cout << "\tpaletteCollection ID: " << data->palletteCollectionID << std::endl;
	std::cout << "\tpaletteMap ID: " << data->paletteMapID << std::endl;
}

std::list<std::string>* entityDataExtractor::getPlugins(Json::Value data){
	std::list<std::string>* extractedPlugins = new std::list<std::string>;
	
	unsigned int dataIndex = 0;
	while(data["plugins"][dataIndex]){
		tracker.incrementPlugins();

		Json::Value dataSnippit = data["plugins"][dataIndex];
		extractedPlugins->push_back(dataSnippit.asString());
		dataIndex++;
	}

	return extractedPlugins;
}

std::list<pluginMetadataEntry*>* entityDataExtractor::getPluginMetadata(Json::Value data, std::list<std::string>* plugins){
	const std::string sectionName = "pluginMetadata";
	std::list<pluginMetadataEntry*>* extractedPluginMetadata = new std::list<pluginMetadataEntry*>;

	unsigned int index = 0;
	bool multipleEntries = (data[sectionName].type() == Json::arrayValue);

	for(std::string plugin : *plugins){
		Json::Value dataSnippit;
		if(multipleEntries){
			if(!data[sectionName][index].isMember(plugin)){
				index++;
				continue;
			}

			dataSnippit = data[sectionName][index][plugin];
			index++;
		}
		else{
			if(!data[sectionName].isMember(plugin))  continue;
			dataSnippit = data[sectionName][plugin];
		}
		
		pluginMetadataEntry* entry = extractPluginMetadataByType(dataSnippit);
		entry->version = dataSnippit["version"].asString();

		extractedPluginMetadata->push_back(entry);
	}

	return extractedPluginMetadata;
}
pluginMetadataEntry* entityDataExtractor::extractPluginMetadataByType(Json::Value pluginData){
	if(pluginData.isMember("objectType")){
		pluginMetadataFraymakersObject* pluginMetadataEntry = new pluginMetadataFraymakersObject;

		pluginMetadataEntry->objectType = translator.toObjectEnum(pluginData["objectType"].asString());
		return pluginMetadataEntry;
	}
	else if(pluginData.isMember("spritesheetGroup")){
		pluginMetadataFraymakersMenu* pluginMetadataEntry = new pluginMetadataFraymakersMenu;

		pluginMetadataEntry->spritesheetGroup = pluginData["spritesheetGroup"].asString();
		return pluginMetadataEntry;
	}

	return nullptr;
}


std::list<symbol*>* entityDataExtractor::getSymbols(Json::Value data){
	const std::string sectionName = "symbols";
	std::list<symbol*> *extractedSymbols = new std::list<symbol*>;

	unsigned int dataIndex = 0;
	while(data[sectionName][dataIndex]){
		Json::Value dataSnippit = data[sectionName][dataIndex];
		symbol* entry = extractSymbolByType(dataSnippit);

		entry->id = dataSnippit["$id"].asString();
		entry->alpha =
			(dataSnippit["alpha"] == Json::Value::null) ?
			-1.0 :
			dataSnippit["alpha"].asFloat();

		extractedSymbols->push_back(entry);
		tracker.incrementSymbols();

		dataIndex++;
	}

	return extractedSymbols;
}
symbol* entityDataExtractor::extractSymbolByType(Json::Value symbolData){
	SYMBOL_TYPE type = translator.toSymbolEnum(symbolData["type"].asString());
	
	if(type == SYM_IMAGE){
		tracker.incrementImageSymbols();

		symbolImage* symbol = new symbolImage;
		symbol->imageAssetID = symbolData["imageAsset"].asString();
		symbol->x = symbolData["x"].asFloat();
		symbol->y = symbolData["y"].asFloat();
		symbol->rotation = symbolData["rotation"].asFloat();
		symbol->scaleX = symbolData["scaleX"].asFloat();
		symbol->scaleY = symbolData["scaleY"].asFloat();
		symbol->pivotX = symbolData["pivotX"].asFloat();
		symbol->pivotY = symbolData["pivotY"].asFloat();
		return symbol;
	}
	else if(type == SYM_COLLISION_BOX){
		tracker.incrementCollisionBoxSymbols();
		
		symbolCollisionBox* symbol = new symbolCollisionBox;
		symbol->color =
			(symbolData["color"].asString().empty()) ?
			"null" :
			symbolData["color"].asString();
		symbol->x = symbolData["x"].asFloat();
		symbol->y = symbolData["y"].asFloat();
		symbol->rotation = symbolData["rotation"].asFloat();
		symbol->scaleX = symbolData["scaleX"].asFloat();
		symbol->scaleY = symbolData["scaleY"].asFloat();
		symbol->pivotX = symbolData["pivotX"].asFloat();
		symbol->pivotY = symbolData["pivotY"].asFloat();
		return symbol;
	}
	else if(type == SYM_COLLISION_BODY){
		tracker.incrementCollisionBodySymbols();

		symbolCollisionBody* symbol = new symbolCollisionBody;
		symbol->color =
			(symbolData["color"].asString().empty()) ?
			"null" :
			symbolData["color"].asString();
		symbol->head = symbolData["head"].asFloat();
		symbol->hipWidth = symbolData["hipWidth"].asFloat();
		symbol->hipXOffset = symbolData["hipXOffset"].asFloat();
		symbol->hipYOffset = symbolData["hipYOffset"].asFloat();
		symbol->foot = symbolData["foot"].asFloat();
		return symbol;
	}
	else if(type == SYM_POINT){
		tracker.incrementPointSymbols();

		symbolPoint* symbol = new symbolPoint;
		symbol->color = symbolData["color"].asString();
		symbol->x = symbolData["x"].asFloat();
		symbol->y = symbolData["y"].asFloat();
		symbol->rotation = symbolData["rotation"].asFloat();
		return symbol;
	}
	else if(type == SYM_LINE_SEGMENT){
		tracker.incrementLineSegmentSymbols();

		symbolLineSegment* symbol = new symbolLineSegment;
		symbol->color = symbolData["color"].asString();
		symbol->x1 = symbolData["points"][0].asFloat();
		symbol->y1 = symbolData["points"][1].asFloat();
		symbol->x2 = symbolData["points"][2].asFloat();
		symbol->y2 = symbolData["points"][3].asFloat();
		return symbol;
	}

	return nullptr;
}
void entityDataExtractor::printData(std::list<symbol*>* data){
	printTitle("Symbols");
	for(symbol* entry : *data){
		std::cout << "\tid: " << entry->id << std::endl;
		std::cout << "\talpha: " << entry->alpha << std::endl;

		printData(entry);

		std::cout << std::endl;
	}
}
void entityDataExtractor::printData(symbol* data){
	if(symbolImage* image = dynamic_cast<symbolImage*>(data)){
		std::cout << "\tx: " << image->x << std::endl;
		std::cout << "\ty: " << image->y << std::endl;
		std::cout << "\trotation: " << image->rotation << std::endl;
		std::cout << "\tscaleX: " << image->scaleX << std::endl;
		std::cout << "\tscaleY: " << image->scaleY << std::endl;
		std::cout << "\tpivotX: " << image->pivotX << std::endl;
		std::cout << "\tpivotY: " << image->pivotY << std::endl;
	}
	else if(symbolCollisionBox* collisionBox = dynamic_cast<symbolCollisionBox*>(data)){
		std::cout << "\tcolor: " << collisionBox->color << std::endl;
		std::cout << "\tx: " << collisionBox->x << std::endl;
		std::cout << "\ty: " << collisionBox->y << std::endl;
		std::cout << "\trotation: " << collisionBox->rotation << std::endl;
		std::cout << "\tscaleX: " << collisionBox->scaleX << std::endl;
		std::cout << "\tscaleY: " << collisionBox->scaleY << std::endl;
		std::cout << "\tpivotX: " << collisionBox->pivotX << std::endl;
		std::cout << "\tpivotY: " << collisionBox->pivotY << std::endl;
	}
	else if(symbolCollisionBody* collisionBody = dynamic_cast<symbolCollisionBody*>(data)){
		std::cout << "\tcolor: " << collisionBody->color << std::endl;
		std::cout << "\thead: " << collisionBody->head << std::endl;
		std::cout << "\thipWidth: " << collisionBody->hipWidth << std::endl;
		std::cout << "\thipXOffset: " << collisionBody->hipXOffset << std::endl;
		std::cout << "\thipYOffset: " << collisionBody->hipYOffset << std::endl;
		std::cout << "\tfoot: " << collisionBody->foot << std::endl;
	}
	else if(symbolPoint* point = dynamic_cast<symbolPoint*>(data)){
		std::cout << "\tcolor: " << point->color << std::endl;
		std::cout << "\tx: " << point->x << std::endl;
		std::cout << "\ty: " << point->y << std::endl;
		std::cout << "\trotation: " << point->rotation << std::endl;
	}
	else if(symbolLineSegment* lineSegment = dynamic_cast<symbolLineSegment*>(data)){
		std::cout << "\tcolor: " << lineSegment->color << std::endl;
		std::cout << "\tx1: " << lineSegment->x1 << std::endl;
		std::cout << "\ty1: " << lineSegment->y1 << std::endl;
		std::cout << "\tx2: " << lineSegment->x2 << std::endl;
		std::cout << "\ty2: " << lineSegment->y2 << std::endl;
	}
}

std::list<std::string> entityDataExtractor::getTags(Json::Value data){
	const std::string sectionName = "tags";
	std::list<std::string> extractedTags;

	if(!data[sectionName][0]){
		extractedTags.push_back("null");
		return extractedTags;
	}
	
	unsigned int dataIndex = 0;
	while(data[sectionName][dataIndex]){
		extractedTags.push_back(data[sectionName][dataIndex].asString());
		dataIndex++;
	}

	return extractedTags;
}

void entityDataExtractor::printTitle(std::string title){
	std::cout << "=========================================\n";
	std::cout << title << std::endl;
	std::cout << "=========================================\n";
}