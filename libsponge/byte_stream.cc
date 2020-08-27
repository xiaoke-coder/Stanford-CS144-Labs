#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity): _capacity(capacity),_used(0),_data(""),
_input_ended(false),_byte_written(0),
_byte_read(0),_eof(false){} //DUMMY_CODE(capacity); }

size_t ByteStream::write(const string &data) {
    //DUMMY_CODE(data);
    size_t r_capacity=remaining_capacity();
    if(r_capacity==0)
        return 0;
    size_t write_size=min(data.length(),r_capacity);
    _data.append(data,0,write_size);
    _byte_written+=write_size;
    _used+=write_size;
    return write_size;
    //return {};
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    //DUMMY_CODE(len);
    size_t true_len=min(len,_used);
    return _data.substr(0,true_len);
    //return {};
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    // DUMMY_CODE(len); 
    size_t true_len=min(len,_used);
    _data=_data.substr(true_len,_used-true_len);
    _used-=true_len;
    _byte_read+=true_len;
    if(buffer_empty()&&input_ended())
    _eof = true;
    }

void ByteStream::end_input() {
    _input_ended = true;
    if(buffer_empty())
    _eof=true;
}

bool ByteStream::input_ended() const { return _input_ended;}
    //return {}; }

size_t ByteStream::buffer_size() const { return _used; }

bool ByteStream::buffer_empty() const { return _used == 0; }

bool ByteStream::eof() const { return _eof; }

size_t ByteStream::bytes_written() const { return _byte_written; }

size_t ByteStream::bytes_read() const { return _byte_read; }

size_t ByteStream::remaining_capacity() const { return _capacity-_used; }
