#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <vector>

enum {
    MAX_SIZE = 50000,
    AMOUNT = 1000000
};

struct fileInfo {
    std::ifstream reading;
    std::ofstream writing;
    std::string name;
    uint64_t len = 0;
};

using fileInfo = struct fileInfo;

void Merge(std::vector<uint64_t>& arr, uint64_t l, uint64_t m, uint64_t r) {
    uint64_t i, j, k;
    uint64_t n1 = m - l + 1;
    uint64_t n2 = r - m;

    std::vector<uint64_t> L(n1);
    std::vector<uint64_t> R(n2);

    for (i = 0; i < n1; i++) {
	    L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++) {
	    R[j] = arr[m + 1 + j];
    }

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
	    if (L[i] <= R[j]) {
       	    arr[k++] = L[i++];
	    }
	    else {
	        arr[k++] = R[j++];
	    }
    }

    while (i < n1) {
	    arr[k++] = L[i++];
    }
    while (j < n2) {
	    arr[k++] = R[j++];
    }
}

void MergeSort(std::vector<uint64_t>& arr, uint64_t l, uint64_t r) {
    if (l < r) {
        uint64_t m = l + (r - l) / 2;
        MergeSort(arr, l, m);
        MergeSort(arr, m + 1, r);
        Merge(arr, l, m, r);
    }
}

uint64_t amountNumbers(void) {
    std::ifstream box("input.bin", std::ios_base::binary);
    uint64_t tmp;
    uint64_t len = 0;
    while (!box.eof()) {
        box >> tmp;
        len++;
    }
    return len;
}

std::vector<fileInfo> placeFiles(uint64_t quant) {
    std::ifstream fin("input.bin", std::ios_base::binary);;
    std::vector<fileInfo> files(quant);
    std::vector<uint64_t> box(MAX_SIZE);
    for (uint64_t i = 0; i < quant; i++) {
        while (!fin.eof() && files[i].len < MAX_SIZE) {
            fin >> box[files[i].len];
            files[i].len++;
        }
        MergeSort(box, 0, files[i].len - 1);
        files[i].name = std::to_string(i) + ".bin";
        files[i].writing.open(files[i].name, std::ios_base::binary);
        for (uint64_t j = 0; j < files[i].len; j++)
            files[i].writing << box[j] << '\n';
        files[i].writing.close();
    }
    return files;
}

fileInfo& mergeFiles(fileInfo& left, fileInfo& right, uint64_t side) {
    std::string name = "tmp" + std::to_string(side) + ".bin";
    std::ofstream writeBox(name, std::ios_base::binary);
    left.reading.open(left.name, std::ios_base::binary);
    right.reading.open(right.name, std::ios_base::binary);

    uint64_t lBox;
    bool lFlag = true;
    uint64_t rBox;
    bool rFlag = true;
    uint64_t leftLen = left.len;
    uint64_t rightLen = right.len;
    uint64_t allLen = leftLen + rightLen;

    while (leftLen && rightLen) {
	if (lFlag) {
	    left.reading >> lBox;
	    lFlag = false;
	}
	if (rFlag) {
	    right.reading >> rBox;
	    rFlag = false;
	}
	if (lBox <= rBox) {
	    writeBox << lBox << '\n';
	    lFlag = true;
	    leftLen--;
	}
	else {
	    writeBox << rBox << '\n';
	    rFlag = true;
	    rightLen--;
	}
    }

    if (!lFlag && leftLen) {
        writeBox << lBox << '\n';
        leftLen--;
    }
    if (!rFlag && rightLen) {
        writeBox << rBox << '\n';
        rightLen--;
    }

    while (leftLen) {
        left.reading >> lBox;
		writeBox << lBox << '\n';
		leftLen--;
	}
	while (rightLen) {
        right.reading >> rBox;
        writeBox << rBox << '\n';
        rightLen--;
	}

	left.reading.close();
	right.reading.close();
	writeBox.close();

	std::ifstream readBox(name, std::ios_base::binary);
	left.writing.open(left.name, std::ios_base::binary);
	while (allLen) {
        readBox >> lBox;
        left.writing << lBox << '\n';
        allLen--;
    }
    readBox.close();
    left.writing.close();
    left.len += right.len;
    remove(name.c_str());
    return left;
}

fileInfo& concatenateFiles(std::vector<fileInfo>& files, uint64_t l, uint64_t r, const uint64_t side) {
    if (l < r) {
	uint64_t m = l + (r - l) / 2;
	fileInfo& left = concatenateFiles(files, l, m, side);
	fileInfo& right = concatenateFiles(files, m + 1, r, side);
	return mergeFiles(left, right, side);
	}
    return files[l];
}

void removeAll(std::vector<fileInfo>& files, uint64_t quant) {
    for (uint64_t i = 1; i < quant; i++) {
        remove(files[i].name.c_str());
    }
}

void createHuge(void) {
    std::ofstream file("input.bin", std::ios_base::binary);
    for (uint64_t i = AMOUNT; i > 0; i--) {
        file << i;
        if (i != 1)
            file << '\n';
        if (i % 100000 == 0)
            std::cout << i << '\n';
    }
    file.close();
}

int main(void) {
    //createHuge();
    uint64_t len = amountNumbers();
    uint64_t quant = len / MAX_SIZE;
    if (len % MAX_SIZE)
        quant++;
    std::vector<fileInfo> files = placeFiles(quant);
    if (quant > 1) {
        uint64_t m = quant / 2 - 1;
        std::thread left(concatenateFiles, std::ref(files), 0, m, 1);
        std::thread right(concatenateFiles, std::ref(files), m + 1, quant - 1, 2);
        left.join();
        right.join();
        mergeFiles(files[0], files[m + 1], 0);
    }
    removeAll(files, quant);

    return 0;
}
