#from tornado.httputil import HTTPServerRequest
import tornado.web
from tornado.httpserver import HTTPServer
import tornado.ioloop
from loguru import logger
import json
from inference import evaluate
#import os
#import numpy as np
#os.environ["CUDA_VISIBLE_DEVICES"] = '2'
import torch
import tornado
import tornado.gen
from tornado.concurrent import run_on_executor
from concurrent.futures import ThreadPoolExecutor
from transformers import AlbertForSequenceClassification,BertTokenizerFast
# model_path = '/home/yuanchaoyi/BeiKe/QA_match/roberta_base'
model_path = "clue/albert_chinese_tiny"
tokenizer = BertTokenizerFast.from_pretrained(model_path,do_lower_case= True)
# from data_process import yeild_test_data_acc
#device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
model_save_path = './save_model/senstive_al.pth'
# test_data = './train_zh_senstive/test_all'
model = AlbertForSequenceClassification.from_pretrained(model_path,num_labels=2)
model.load_state_dict(torch.load(model_save_path,map_location=torch.device('cpu')))
class DemoModelServer(tornado.web.RequestHandler):
    executor = ThreadPoolExecutor(100)
    @run_on_executor
    def inference_task(self,sentence):
        result = evaluate(sentence,model,tokenizer)
        return result
    @tornado.gen.coroutine
    def post(self):
        # 获取post方式的参数
        recordid = self.get_argument("recordid", "TEST")
        sentence = self.get_argument("sentence", -1)
        request_body = json.loads(self.request.body.decode('utf-8'))
        recordid = request_body.get("recordid", "TEST")
        sentence = request_body.get("sentence", -1)
        logger.info("[{}]request: {}".format(recordid, sentence))
        # 模型预测
        result = yield self.inference_task(sentence)
        #print(result)
        # 结果整理和返回
        output_result = {"recordid": recordid, "result": result}
        logger.info("[{}]response: {}".format(recordid, output_result))
        self.write(json.dumps(output_result))
    #@run_on_executor
    #def inference_task(self,sentence):
    #    result = evaluate(sentence,model,tokenizer)
    #    return result

if __name__ == '__main__':
    #创建一个应用对象
    #tornado.options.parse_command_line()
    #tornado.options.parse_command_line()
    app = tornado.web.Application([(r'/demo',DemoModelServer)],autoreload=False, debug=False)
    #绑定一个监听端口
    app.listen(8081)
    #server = HTTPServer(app)
    #server.bind(8081)
    #server.start(1)
    #tornado.ioloop.IOLoop.instance().start()
    #启动web程序，开始监听端口的连接
    # Run.instance().start()
    tornado.ioloop.IOLoop.current().start()
