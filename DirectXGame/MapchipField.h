#pragma once
#include<vector>
#include<map>
#include<string>
#include"Vector3.h"
#include <cstdint>

enum class MapChipType {
	kBlank,
	kBlock,

};
struct MapChipData {
	std::vector<std::vector<MapChipType>> data;

};


class MapChipField {
public:
	MapChipData mapChipData_;
	void ResetMapChipDate();
    void LoadMapChipCsv(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMaoChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	

private:
	//1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;
	//ブロックの個数
	static inline const int kNumBlockVirtical = 20;
	static inline const int kNumBlockHorizontal = 100;
	//インラインがあればメインにもう一回書かなくていい

};


