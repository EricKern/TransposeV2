#include <iostream>
#include <oneapi/tbb.h>

#define DATA_POLICY static
#define TRANSPOSE_POLICY static

using DataPartitioner = oneapi::tbb::simple_partitioner;
static DataPartitioner dataPart;

using TransposePartitioner = oneapi::tbb::simple_partitioner;
static TransposePartitioner transPart;

using InValType = float;

#include "dataV2.hpp"
#include "transpose.hpp"

static const size_t in_rows = 7;
static const size_t in_cols = 5;
static const size_t tilesize = 4;

int main(int argc, char const *argv[])
{
    // pad::arrayDataV2<InValType> dataS(4, 3);
    // dataS.printA();
    // dataS.printB();

    // pad::arrayDataV2<InValType> dataOMP(11, 13, 4, "OMP");
    // dataOMP.printA();
    // dataOMP.printB();

    pad::arrayDataV2<InValType> dataTBB(in_rows, in_cols, tilesize, "TBB", dataPart);
    dataTBB.printA();
    

    auto iterators = dataTBB.get_range();
    auto [beginA, endA] = std::get<0>(iterators);
    auto [beginB, endB] = std::get<1>(iterators);
    auto [dataA, dataB] = dataTBB.get_ptr();

    // transpose::transposeSerial(beginA, beginB, endB, in_rows);
    // std::execution::par_unseq,
    // transpose::stl_like(beginA, endA, beginB, in_cols);
    // transpose::stl_likeCoalescedWrite(beginA, beginB, endB, in_cols);
    transpose::tbb(beginA, beginB, endB, in_rows, tilesize, transPart);
    transpose::verifyT(beginA, beginB, endB, in_rows);
    dataTBB.printB();

    // std::cout << "Hello World";
    return 0;
}
