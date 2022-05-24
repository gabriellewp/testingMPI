#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


struct header{
    header(): version(0), dt(0), nrows(0), ncolumns(0){}
    uint8_t version;
    uint8_t dt;
    uint64_t nrows;
    uint64_t ncolumns;
};

struct block{
    block(): nrows(0), n_cols(0), bt(0){}
    uint32_t nrows;
    uint32_t ncols;
    uint8_t bt; //block type
    uint8_t* btinfo;
};   

struct body: block{
    body(): irow(0), icol(0){}
    uint64_t irow;
    uint64_t icol;
    block block_;
};  

struct Message: header, block, body
{
    message(): header(), body(), block(){} // Added default constructor
    
    header header_;
    body body_;
    block block_;

    void serialize(Archive& ar, const unsigned int version)
    {
        ar & header_;
        ar & body_;
        ar & block_;
    }
}