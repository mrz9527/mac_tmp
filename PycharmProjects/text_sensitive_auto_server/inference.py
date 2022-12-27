# import os
import numpy as np
# import tensorrt
# import torch_tensorrt
# import numpy as np
# import torch.backends.cudnn as cudnn
# torch.hub._validate_not_a_forked_repo=lambda a,b,c: True
# import time
# os.environ["CUDA_VISIBLE_DEVICES"] = '2'
import torch
# cudnn.benchmark = True
# from transformers import AlbertForSequenceClassification,BertTokenizerFast,BertConfig
# # model_path = '/home/yuanchaoyi/BeiKe/QA_match/roberta_base'
# model_path = "clue/albert_chinese_tiny"
# tokenizer = BertTokenizerFast.from_pretrained(model_path,do_lower_case= True)
# # from data_process import yeild_test_data_acc
device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
# model_save_path = './save_model/senstive_al.pth'
# # test_data = './train_zh_senstive/test_all'
# config = BertConfig(vocab_size_or_config_json_file=22606, hidden_size=312,
#     num_hidden_layers=4, num_attention_heads=12, intermediate_size=1248, torchscript=True)
# model = AlbertForSequenceClassification.from_config(config)
# model = AlbertForSequenceClassification.from_pretrained(model_path,num_labels=2,torchscript=True)
# model.load_state_dict(torch.load(model_save_path))
# model.to(device)
maxlen= 250
# test_dataloader = yeild_test_data_acc(test_data)
def evaluate(sentence, model,tokenizer):
    model.to(device)
    model.eval()
    data = tokenizer(sentence,return_offsets_mapping=True,max_length=maxlen,truncation=True,padding='max_length',return_tensors='pt')
    with torch.no_grad():
        # for data,label in dataloader:
        input_ids = data['input_ids'].to(device)
        attention_mask = data['attention_mask'].to(device)
        token_type_ids = data['token_type_ids'].to(device)
        # label = label.reshape(-1,1).to(device)
        # traced_model = torch.jit.trace(model, [input_ids, token_type_ids,attention_mask])
        model_out = model(input_ids,attention_mask,token_type_ids)
        # val_loss += model_out.loss
        pred = model_out.logits.detach().cpu().numpy()
        pred = np.argmax(pred, axis=1)[0]
        # f1 += flat_accuracy(pred,label.detach().cpu().numpy())
        # f1_mean = f1 / size
        # val_loss = val_loss / size
    # print(f"F1:{round(f1,2):>2f},Avg loss: {val_loss:>8f} \n")
    if pred== 1:
        pred ='敏感'
    else:
        pred = '正常'
    return pred
# if __name__ == '__main__':
#     while True:
#         sentence = input("输入一句话:")
#         time_start=time.time()
#         # sentence = "放你妈的屁，这个婊子该被人干死"
#         f1_mean = evaluate(sentence,model)
#         time_end=time.time()
#         print('time cost',time_end-time_start,'s')