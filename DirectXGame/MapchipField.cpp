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
Vector3 MapChipField::GetMaoChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVirtical - 1 - yIndex), 0); }//ここの数字がエグイ遠くなるらしいけどこれがどうなってるのやら


uint32_t MapChipField::GetNumBlockVirtical() { return kNumBlockVirtical; }

uint32_t MapChipField::GetNumBlockHorizontal() { return kNumBlockHorizontal; }

IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	int32_t xIndex = static_cast<int32_t>((position.x + kBlockWidth / 2) / kBlockWidth);//待っぷっちぷのインデックスに直してう（マップチップの何番目七日になおしてる）
	int32_t yIndex = static_cast<int32_t>((position.y + kBlockHeight / 2) / kBlockHeight);
	int32_t maxIndexY = kNumBlockVirtical - 1;
	yIndex = maxIndexY - yIndex;//ここがけーむの原点に合わしてる

	IndexSet indexSet;
	indexSet.xIndex = xIndex;
	indexSet.yIndex = yIndex;

	    return indexSet;
}

Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) { 

Vector3 center = GetMaoChipPositionByIndex(xIndex, yIndex);

Rect rect;
rect.left = center.x - kBlockWidth / 2.0f;
rect.right = center.x + kBlockWidth / 2.0f;
rect.bottom = center.y - kBlockHeight / 2.0f;
rect.top = center.y + kBlockHeight / 2.0f;

return rect;
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVirtical - 1 < yIndex) {//溢れたら面倒だからマイナス一
		return MapChipType::kBlank;
	}

	 return mapChipData_.data[yIndex][xIndex];
	//return MapChipType();
}//関数自体はおｋ



