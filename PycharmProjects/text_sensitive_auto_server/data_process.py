from torch.utils.data import Dataset,DataLoader
from transformers import BertTokenizerFast
import re
# import emoji
# model_path = '/home/yuanchaoyi/BeiKe/QA_match/roberta_base'
model_path = '/home/yuanchaoyi/albert_chinese_tiny'
batch_size = 142
tokenizer = BertTokenizerFast.from_pretrained(model_path)
# def remove_emoji(text, replace=""):
    # return re.sub(emoji.get_emoji_regexp(), replace, text)
def load_data(file,file2):
    with open(file) as train_pos:
        with open(file2) as train_neg:
            D = []
            D_dev = []
            for num,line in enumerate(train_pos):
                line = re.sub('\n','',line)
                # line = emoji.demojize(line)
                # line = remove_emoji(line)
                if num % 7 == 0:
                    D_dev.append((line,0))
                else:
                    D.append((line,0))
            for num,line in enumerate(train_neg):
                line = re.sub('\n','',line)
                # line = emoji.demojize(line)
                # line = remove_emoji(line)
                if num % 7 == 0:
                    D_dev.append((line,1))
                else:
                    D.append((line,1))
    return D,D_dev
def load_test(file):
     with open(file) as test_data:
        D = []
        for line in test_data:
            line = re.sub('\n','',line)
            if line != '':
                D.append(line)
        return D
def load_test_acc(file):
    with open(file) as test_data:
        D = []
        for line in test_data:
            line = re.sub('\n','',line)
            line = line.split('\t')
            if line != '':
                D.append((line[0],eval(line[1])))
        return D
class TorchData(Dataset):
    def __init__(self,data,is_train):
        self.data = data
        self.maxlen = 250
        self.is_train = is_train
    def __len__(self):
        return len(self.data)
    def __getitem__(self, index):
        data = self.data[index]
        # if data[2] != 0.0 and data[2] != 1.0:
        if self.is_train == 'train_dev':
            context = tokenizer(data[0],return_offsets_mapping=True,max_length=self.maxlen,truncation=True,padding='max_length',return_tensors='pt')
            label = data[1]
            return context,label
        elif self.is_train == 'test':
            context = tokenizer(data,return_offsets_mapping=True,max_length=self.maxlen,truncation=True,padding='max_length',return_tensors='pt')
            return context,data
        
def yeild_data(train_file_data_pos,train_file_data_neg):
    train_data,dev_data = load_data(train_file_data_pos,train_file_data_neg)
    train_data = TorchData(train_data,is_train='train_dev')
    dev_data = TorchData(dev_data,is_train='train_dev')
    train_dataloader = DataLoader(train_data, batch_size=batch_size,drop_last=True,shuffle=True)
    dev_dataloader = DataLoader(dev_data, batch_size=batch_size,drop_last=True,shuffle=True)
    return train_dataloader,dev_dataloader
def yeild_test_data(test_file):
    test_data = load_test(test_file)
    test_data = TorchData(test_data,is_train='test')
    test_dataloader = DataLoader(test_data,batch_size=batch_size,shuffle=True)
    return test_dataloader
def yeild_test_data_acc(test_file):
    test_data = load_test_acc(test_file)
    test_data = TorchData(test_data,is_train='train_dev')
    test_dataloader = DataLoader(test_data,batch_size=batch_size,shuffle=True)
    return test_dataloader
def test_data(file):
    # with open('./train_zh_senstive/train_dirty_39w','w',encoding='utf8') as train_file:
    with open('./train_zh_senstive/test_all','a',encoding='utf8') as test_file: 
        with open(file) as f:
            for num,line in enumerate(f.readlines()):
                line = re.sub('\n','',line)
                line = line.split('\t')[0]
                if num >395000:
                    test_file.write(line+'\t'+'0'+'\n')
                    # else:
                        # train_file.write(line+'\n')
    # return 0

# test_data('./train_zh_senstive/train_clean')