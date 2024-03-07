#define HTTP_PACKET_BUF_SIZE 20

struct httpPacket{
        bool post_request = false;
        bool get_request = false;
        String uri = "";

        int content_length = 0;

        int post_len = 0;
        String post_key[HTTP_PACKET_BUF_SIZE];
        String post_val[HTTP_PACKET_BUF_SIZE];

        int get_len = 0;
        String get_key[HTTP_PACKET_BUF_SIZE];
        String get_val[HTTP_PACKET_BUF_SIZE];

	int cookie_len = 0;
	String cookie_key[HTTP_PACKET_BUF_SIZE];
	String cookie_val[HTTP_PACKET_BUF_SIZE];

        bool error = false;
};
typedef struct httpPacket HttpPacket;
