
#include <tp/gzipio.h>
#include <tp/file.h>
#include <tp/log.h>


int main(int argc, char* argv[])
{

    tpFile infile;

    if (!infile.open(argv[1],"rb")) return -1;
    tpGZipIO zipstream;

    tpArray<tpChar> buffer; buffer.resize(16384);

    zipstream.open(&infile);

    while (infile.isGood() && zipstream.isGood()) {

        zipstream.read(buffer.getData(),buffer.getSize());

        tpLogNotify("\n%s\n",buffer.getData());
    }

    return 0;
}
