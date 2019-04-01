typedef struct PrpcHead_t {
	uint8_t magic[4];      //'P','R','P','C'
	uint32_t length;
	uint32_t meta_length;
}PrpcHead;


#endif


