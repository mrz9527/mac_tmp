FFMPEG中结构体很多。最关键的结构体可以分成以下几类：
 解协议（http,rtsp,rtmp,mms）
 AVIOContext，URLProtocol，URLContext主要存储视音频使用的协议的类型以及状态。URLProtocol存储输入视音频使用的封装格式。每种协议都对应一个URLProtocol结构。（注意：FFMPEG中文件也被当做一种协议“file”）
 解封装（flv,avi,rmvb,mp4）
 AVFormatContext主要存储视音频封装格式中包含的信息；AVInputFormat存储输入视音频使用的封装格式。每种视音频封装格式都对应一个AVInputFormat 结构。
 解码（h264,mpeg2,aac,mp3）
 每个AVStream存储一个视频/音频流的相关数据；每个AVStream对应一个AVCodecContext，存储该视频/音频流使用解码方式的相关数据；每个AVCodecContext中对应一个AVCodec，包含该视频/音频对应的解码器。每种解码器都对应一个AVCodec结构。
 存数据
 视频的话，每个结构一般是存一帧；音频可能有好几帧
 解码前数据：AVPacket
 解码后数据：AVFrame

## AVCodec

AVCodec是存储编解码器信息的结构体
 const char *name：编解码器的名字，比较短
 const char *long_name：编解码器的名字，全称，比较长
 enum AVMediaType type：指明了类型，是视频，音频，还是字幕
 enum AVCodecID id：ID，不重复
 const AVRational *supported_framerates：支持的帧率（仅视频）
 const enum AVPixelFormat *pix_fmts：支持的像素格式（仅视频）
 const int *supported_samplerates：支持的采样率（仅音频）
 const enum AVSampleFormat *sample_fmts：支持的采样格式（仅音频）
 const uint64_t *channel_layouts：支持的声道数（仅音频）
 int priv_data_size：私有数据的大小
 1.注册所有编解码器：av_register_all();
 2.声明一个AVCodec类型的指针，比如说AVCodec* first_c;
 3.调用av_codec_next()函数，即可获得指向链表下一个解码器的指针，循环往复可以获得所有解码器的信息。注意，如果想要获得指向第一个解码器的指针，则需要将该函数的参数设置为NULL。

## AVCodecContext

这是一个描述编解码器上下文的数据结构，包含了众多编解码器需要的参数信息
 如果是单纯使用libavcodec，这部分信息需要调用者进行初始化；如果是使用整个FFMPEG库，这部分信息在调用 av_open_input_file和av_find_stream_info的过程中根据文件的头信息及媒体流内的头部信息完成初始化。其中几个主要 域的释义如下：
 extradata/extradata_size： 这个buffer中存放了解码器可能会用到的额外信息，在av_read_frame中填充。一般来说，首先，某种具体格式的demuxer在读取格式头 信息的时候会填充extradata，其次，如果demuxer没有做这个事情，比如可能在头部压根儿就没有相关的编解码信息，则相应的parser会继 续从已经解复用出来的媒体流中继续寻找。在没有找到任何额外信息的情况下，这个buffer指针为空。
 time_base：
 width/height：视频的宽和高。
 sample_rate/channels：音频的采样率和信道数目。
 sample_fmt： 音频的原始采样格式。
 codec_name/codec_type/codec_id/codec_tag：编解码器的信息。

## AVStream

该结构体描述一个媒体流
 主要域的释义如下，其中大部分域的值可以由av_open_input_file根据文件头的信息确定，缺少的信息需要通过调用av_find_stream_info读帧及软解码进一步获取：
 index/id：index对应流的索引，这个数字是自动生成的，根据index可以从AVFormatContext::streams表中索引到该流；而id则是流的标识，依赖于具体的容器格式。比如对于MPEG TS格式，id就是pid。
 time_base：流的时间基准，是一个实数，该流中媒体数据的pts和dts都将以这个时间基准为粒度。通常，使用av_rescale/av_rescale_q可以实现不同时间基准的转换。
 start_time：流的起始时间，以流的时间基准为单位，通常是该流中第一个帧的pts。
 duration：流的总时间，以流的时间基准为单位。
 need_parsing：对该流parsing过程的控制域。
 nb_frames：流内的帧数目。
 r_frame_rate/framerate/avg_frame_rate：帧率相关。
 codec：指向该流对应的AVCodecContext结构，调用av_open_input_file时生成。
 parser：指向该流对应的AVCodecParserContext结构，调用av_find_stream_info时生成。

## AVFormatContext

这个结构体描述了一个媒体文件或媒体流的构成和基本信息
 这是FFMpeg中最为基本的一个结构，是其他所有结构的根，是一个多媒体文件或流的根本抽象。其中:nb_streams和streams所表示的AVStream结构指针数组包含了所有内嵌媒体流的描述；iformat和oformat指向对应的demuxer和muxer指针；pb则指向一个控制底层数据读写的ByteIOContext结构。
 start_time和duration是从streams数组的各个AVStream中推断出的多媒体文件的起始时间和长度，以微妙为单位。
 通常，这个结构由av_open_input_file在内部创建并以缺省值初始化部分成员。但是，如果调用者希望自己创建该结构，则需要显式为该结构的一些成员置缺省值——如果没有缺省值的话，会导致之后的动作产生异常。以下成员需要被关注：
 probesize
 mux_rate
 packet_size
 flags
 max_analyze_duration
 key
 max_index_size
 max_picture_buffer
 max_delay

## AVPacket

AVPacket定义在avcodec.h中
 FFMPEG使用AVPacket来暂存解复用之后、解码之前的媒体数据（一个音/视频帧、一个字幕包等）及附加信息（解码时间戳、显示时间戳、时长等）。其中：
 dts 表示解码时间戳，pts表示显示时间戳，它们的单位是所属媒体流的时间基准。
 stream_index 给出所属媒体流的索引；
 data 为数据缓冲区指针，size为长度；
 duration 为数据的时长，也是以所属媒体流的时间基准为单位；
 pos 表示该数据在媒体流中的字节偏移量；
 destruct 为用于释放数据缓冲区的函数指针；
 flags 为标志域，其中，最低为置1表示该数据是一个关键帧。
 AVPacket 结构本身只是个容器，它使用data成员指向实际的数据缓冲区，这个缓冲区可以通过av_new_packet创建，可以通过     av_dup_packet 拷贝，也可以由FFMPEG的API产生（如av_read_frame），使用之后需要通过调用av_free_packet释放。
 av_free_packet调用的是结构体本身的destruct函数，它的值有两种情况：(1)av_destruct_packet_nofree或 0；(2)av_destruct_packet，其中，前者仅仅是将data和size的值清0而已，后者才会真正地释放缓冲区。FFMPEG内部使用 AVPacket结构建立缓冲区装载数据，同时提供destruct函数，如果FFMPEG打算自己维护缓冲区，则将destruct设为 av_destruct_packet_nofree，用户调用av_free_packet清理缓冲区时并不能够将其释放；如果FFMPEG不会再使用 该缓冲区，则将destruct设为av_destruct_packet，表示它能够被释放。对于缓冲区不能够被释放的AVPackt，用户在使用之前 最好调用av_dup_packet进行缓冲区的克隆，将其转化为缓冲区能够被释放的AVPacket，以免对缓冲区的不当占用造成异常错误。而 av_dup_packet会为destruct指针为av_destruct_packet_nofree的AVPacket新建一个缓冲区，然后将原 缓冲区的数据拷贝至新缓冲区，置data的值为新缓冲区的地址，同时设destruct指针为av_destruct_packet。

## AVFrame

构体保存的是解码后和原始的音视频信息。AVFrame通过函数av_frame_alloc()初始化，该函数仅仅分配AVFrame实例本身，而没有分配其内部的缓存。AVFrame实例由av_frame_free()释放；AVFrame实例通常分配一次，重复使用，如分配一个AVFrame实例来保留解码器中输出的视频帧（此时应在恰当的时候使用av_frame_unref()清理参考帧并将AVFrame归零）。该类所描述的数据通常由AVBuffer的API来保存一个引用计数，并保存于AVFrame.buf
 /AVFrame.extended_buf，在至少存在一个参考的时候（如AVFrame.buf[0] != NULL），则该对象被标记为“被引用”。在此情况下，AVFrame所包含的每一组数据必须包含于AVFrame的缓存中。





# FFMpeg 中比较重要的函数以及数据结构如下:

数据结构:
 (1) AVFormatContext
 (2) AVOutputFormat
 (3) AVInputFormat
 (4) AVCodecContext
 (5) AVCodec
 (6) AVFrame
 (7) AVPacket
 (8) AVPicture
 (9) AVStream

初始化函数:
 (1) av_register_all()
 (2) avcodec_open()
 (3) avcodec_close()
 (4) av_open_input_file()
 (5) av_find_input_format()
 (6) av_find_stream_info()
 (7) av_close_input_file()

音视频编解码函数:
 (1) avcodec_find_decoder()
 (2) avcodec_alloc_frame()
 (3) avpicture_get_size()
 (4) avpicture_fill()
 (5) img_convert()
 (6) avcodec_alloc_context()
 (7) avcodec_decode_video()
 (8) av_free_packet()
 (9) av_free()

文件操作:
 (1) avnew_steam()
 (2) av_read_frame()
 (3) av_write_frame()
 (4) dump_format()

其他函数:
 (1) avpicture_deinterlace()
 (2) ImgReSampleContext()