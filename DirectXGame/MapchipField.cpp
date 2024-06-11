#include "MapchipField.h"
#include <cassert>
#include <fstream>
#include <map>
#include <sstream>

namespace {

std::map<std::string, MapChipType> mapchipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};
}

void MapChipField::ResetMapChipDate() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVirtical);
	for (std::vector<MapChipType>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}
// 定義されてない、変数たちと同じクラス内にいなかった
void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	// マップチップデータをリセット
	ResetMapChipDate();

	// ファイルを開く
	std::ifstream file;
	file.open(filePath);
	// cassertが必要
	assert(file.is_open());

	// マップチップCSV
	std::stringstream mapChipCsv;
	// ファイルの内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	// ファイルを閉じる
	file.close();
	// CSV殻まっぷちっプデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		std::string line;
		getline(mapChipCsv, line);
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {

			std::string word;
			// インスタンスが引数リストと一致しない//ダブルとシングルを間違えないように
			getline(line_stream, word, ',');

			if (mapchipTable.contains(word)) {
				mapChipData_.data[i][j] = mapchipTable[word];
			}
		}
	}
}
Vector3 MapChipField::GetMaoChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }

uint32_t MapChipField::GetNumBlockVirtical() { return kNumBlockVirtical; }

uint32_t MapChipField::GetNumBlockHorizontal() { return kNumBlockHorizontal; }

IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	int32_t xIndex = static_cast<int32_t>((position.x + kBlockWidth / 2) / kBlockWidth);
	int32_t yIndex = static_cast<int32_t>((position.y + kBlockHeight / 2) / kBlockHeight);
	int32_t maxIndexY = kNumBlockVirtical - 1;
	yIndex = maxIndexY - yIndex;

	IndexSet indexSet;
	indexSet.xIndex = xIndex;
	indexSet.yIndex = yIndex;

	    return indexSet;
}




MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {
		return MapChipType::kBlank;
	}

	 return mapChipData_.data[yIndex][xIndex];
	//return MapChipType();
}
