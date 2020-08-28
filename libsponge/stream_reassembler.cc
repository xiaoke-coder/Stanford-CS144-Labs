#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : 
    _output(capacity), _capacity(capacity), unassembled_data(_capacity), unassembled_data_exist_flag(_capacity, false) {}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    // DUMMY_CODE(data, index, eof);
    if(eof){
        EOF_Flag = true;
        End_index = index + data.size();
    }

    size_t len = data.size();
    if(Index_after_OrderPart > index+len)   // sequence before Index_after_OrderPart are all valid(ordered and right), this data is repeated
        return ;
    
    size_t RelativeIndex = -1;
    for(size_t i=0; i<len; ++i){
        RelativeIndex = (index+i)%_capacity;
        if( !unassembled_data_exist_flag[RelativeIndex] ){      // only handle incomplete data
            unassembled_data[RelativeIndex] = data[i];
            unassembled_data_exist_flag[RelativeIndex] = true;
            ++unAssembled_size;
        }
    }

    assemble_data();    // put ordered data into ByteStream
}

void StreamReassembler::assemble_data(){
    string Ordered_data = "";
    size_t temp_index = Index_after_OrderPart;

    while( unassembled_data_exist_flag[temp_index % _capacity] ){   // get ordered data from unassembled_data
        unassembled_data_exist_flag[temp_index % _capacity]=false;
        Ordered_data += unassembled_data[temp_index++ % _capacity];
    }

    size_t NumWritten = _output.write(Ordered_data);    // Ordered_data is not exactly write all char with limit of remain buffer size
    size_t len = Ordered_data.size();
    for(size_t i=NumWritten; i<len; ++i){
        unassembled_data_exist_flag[(Index_after_OrderPart+i)%_capacity] = true;    // indicated unhandle
    }
    unAssembled_size -= NumWritten;
    Index_after_OrderPart += NumWritten;

    if(EOF_Flag && Index_after_OrderPart==End_index){
        _output.end_input();    // indicated data in buffer have all transmited
    }
}


size_t StreamReassembler::unassembled_bytes() const { return unAssembled_size; }

bool StreamReassembler::empty() const { return unAssembled_size==0; }
