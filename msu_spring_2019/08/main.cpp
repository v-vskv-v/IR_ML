#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <future>

enum {
    MAX_SIZE = 400
};

struct fInfo {
    std::fstream file;
    std::string name;
    unsigned len = 0;
};

using fileInfo = struct fInfo;

void Merge(uint64_t arr[], unsigned l, unsigned m, unsigned r) {
    unsigned i, j, k;
    unsigned n1 = m - l + 1;
    unsigned n2 = r - m;

    uint64_t L[n1], R[n2];

    for (i = 0; i < n1; i++)
	L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
	R[j] = arr[m + 1+ j];

    i = 0;
    j = 0;
    k = l;

    while (i < n1 && j < n2) {
	if (L[i] <= R[j])
       	    arr[k++] = L[i++];
	else
	    arr[k++] = R[j++];
    }

    while (i < n1) {
	arr[k++] = L[i++];
    }
    while (j < n2) {
	arr[k++] = R[j++];
    }
}

void MergeSort(uint64_t arr[], unsigned l, unsigned r) {
    if (l < r) {
	unsigned m = l + (r - l) / 2;
	MergeSort(arr, l, m);
	MergeSort(arr, m + 1, r);
	Merge(arr, l, m, r);
    }
}

unsigned lenFile(void) {
    std::ifstream box("input.txt");
    unsigned tmp;
    unsigned len = 0;
    while (!box.eof()) {
        box >> tmp;
        len++;
    }
    box.close();
    return len;
}

fileInfo* placeFiles(unsigned quant) {
    std::ifstream fin("input.txt");
    fileInfo* files = new fileInfo[quant];
    uint64_t box[MAX_SIZE];
    for (unsigned i = 0; i < quant; i++) {
        while (!fin.eof() && files[i].len < MAX_SIZE) {
            fin >> box[files[i].len];
            files[i].len++;
        }
        MergeSort(box, 0, files[i].len - 1);
        files[i].name = std::to_string(i) + ".txt";
        files[i].file.open(files[i].name, std::ios_base::out);
        for (unsigned j = 0; j < files[i].len; j++)
            files[i].file << box[j] << ' ';
        files[i].file.close();
    }
    return files;
}

fileInfo& mergeFiles(fileInfo& left, fileInfo& right, unsigned side) {
    std::string name = "tmp" + std::to_string(side) + ".txt";
    std::fstream box(name, std::ios_base::out);
    left.file.open(left.name, std::ios_base::in);
    right.file.open(right.name, std::ios_base::in);

    uint64_t lBox;
    bool lFlag = true;
    uint64_t rBox;
    bool rFlag = true;
    unsigned leftLen = left.len;
    unsigned rightLen = right.len;
    unsigned allLen = leftLen + rightLen;

    while (leftLen && rightLen) {
	if (lFlag) {
	    left.file >> lBox;
	    lFlag = false;
	}
	if (rFlag) {
	    right.file >> rBox;
	    rFlag = false;
	}
	if (lBox <= rBox) {
	    box << lBox << ' ';
	    lFlag = true;
	    leftLen--;
	}
	else {
	    box << rBox << ' ';
	    rFlag = true;
	    rightLen--;
	}
    }

    if (!lFlag && leftLen) {
        box << lBox << ' ';
        leftLen--;
    }
    if (!rFlag && rightLen) {
        box << rBox << ' ';
        rightLen--;
    }

    while (leftLen) {
        left.file >> lBox;
		box << lBox << ' ';
		leftLen--;
	}
	while (rightLen) {
        right.file >> rBox;
        box << rBox << ' ';
        rightLen--;
	}

	left.file.close();
	right.file.close();
	box.close();

	box.open(name, std::ios_base::in);
	left.file.open(left.name, std::ios_base::out);
	while (allLen) {
        box >> lBox;
        left.file << lBox << ' ';
        allLen--;
    }
    box.close();
    left.file.close();
    left.len += right.len;
    return left;
}

fileInfo& concatenateFiles(fileInfo* files, unsigned l, unsigned r, const unsigned side) {
    if (l < r) {
	unsigned m = l + (r - l) / 2;
	fileInfo& left = concatenateFiles(files, l, m, side);
	fileInfo& right = concatenateFiles(files, m + 1, r, side);
	return mergeFiles(left, right, side);
	}
    return files[l];
}

void removeAll(fileInfo* files, unsigned quant) {
    for (unsigned i = 1; i < quant; i++) {
        remove(files[i].name.c_str());
    }
    remove("tmp0.txt");
    remove("tmp1.txt");
    remove("tmp2.txt");
    delete(files);
}


int main(void) {
    unsigned len = lenFile();
    unsigned quant = len / MAX_SIZE;
    if (len % MAX_SIZE)
        quant++;
    fileInfo* files = placeFiles(quant);
    if (quant > 1) {
        unsigned m = quant / 2 - 1;
        std::thread left(concatenateFiles, files, 0, m, 1);
        std::thread right(concatenateFiles, files, m + 1, quant - 1, 2);
        left.join();
        right.join();
        mergeFiles(files[0], files[m + 1], 0);
    }
    removeAll(files, quant);
    return 0;
}
