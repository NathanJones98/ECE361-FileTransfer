
struct packet_format {
    //indicates the total number of fragments of the file
    unsigned int total_frag;

    // sequence number of the fragment
    unsigned int frag_no;

    //size of the data in the range of 0 to 1000
    unsigned int size;

    //ex: “foobar.txt”
    char* filename;

    //ex: “lo World!\n”
    char filedata[1000];
};