#pragma once

/**
 * 计算以 0 结尾字符串占用的数组大小。
 */
template<typename T>
INT WntCountText(CONST T data[]) {
	INT index = 0;
	while (data[index++] != '\0') {}
	return index;
}

/**
 * 克隆数据
 */
template<typename T>
T* WntClone(CONST T data[], INT size) {
	T* result = new T[size];
	for (INT i = 0; i != size; ++i) {
		result[i] = data[i];
	}
	return result;
}