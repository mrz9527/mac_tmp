import os
os.environ["CUDA_VISIBLE_DEVICES"] = '2'
# from torch.utils.data import Dataset,DataLoader
import torch
from transformers import AdamW,AlbertForSequenceClassification,BertConfig
from data_process import yeild_data
from torch.nn.utils import clip_grad_norm_
# import emoji
# model_path = '/home/yuanchaoyi/BeiKe/QA_match/roberta_base'
model_path = "clue/albert_chinese_tiny"
from metrices import flat_accuracy
epochs = 20
clip_norm = 0.25
# gpus = [3,5,6]
# torch.cuda.set_device('cuda:{}'.format(gpus[0]))
train_file_data_pos = '/home/yuanchaoyi/senstive_model/train_zh_senstive/train_clean_39w'
train_file_data_neg = '/home/yuanchaoyi/senstive_model/train_zh_senstive/train_dirty_39w'
model_save_path = './save_model/senstive_al.pth'
learning_rate = 2e-5
device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
train_dataloader,dev_dataloader = yeild_data(train_file_data_pos,train_file_data_neg)
# config = BertConfig(vocab_size_or_config_json_file=22606, hidden_size=312,
#     num_hidden_layers=4, num_attention_heads=12, intermediate_size=1248, torchscript=True)
# model = AlbertForSequenceClassification.from_config(config)
model = AlbertForSequenceClassification.from_pretrained(model_path,num_labels=2)
model.to(device)
no_decay = ['bias', 'gamma', 'beta']
param_optimizer = list(model.named_parameters())
optimizer_grouped_parameters = [
    {'params': [p for n, p in param_optimizer if not any(nd in n for nd in no_decay)],
        'weight_decay_rate': 0.01},
    {'params': [p for n, p in param_optimizer if any(nd in n for nd in no_decay)],
        'weight_decay_rate': 0.0}]
optimizer = AdamW(params=optimizer_grouped_parameters, lr=learning_rate)
def train(dataloader, model,optimizer):
    model.train()
    for batch, (data,label) in enumerate(dataloader):
        input_ids = data['input_ids'].squeeze().to(device)
        attention_mask = data['attention_mask'].squeeze().to(device)
        token_type_ids = data['token_type_ids'].squeeze().to(device)
        label = label.reshape(-1,1).to(device)
        loss = model(input_ids,attention_mask,token_type_ids,labels=label).loss
        # Backpropagation
        loss.backward()
        # fgm.attack()
        # loss_adv = model(input_ids,attention_mask,token_type_ids,labels=label).loss
        # loss_adv.backward()
        # fgm.restore()
        clip_grad_norm_(model.parameters(), max_norm=clip_norm)
        optimizer.step()
        optimizer.zero_grad()
        if batch % 50 == 0:
            loss = loss.item()
            print(f"loss: {loss:>7f}")
def evaluate(dataloader, model):
    model.eval()
    val_loss = 0
    f1 = 0
    size = len(dataloader)
    with torch.no_grad():
        for data,label in dataloader:
            input_ids = data['input_ids'].squeeze().to(device)
            attention_mask = data['attention_mask'].squeeze().to(device)
            token_type_ids = data['token_type_ids'].squeeze().to(device)
            label = label.reshape(-1,1).to(device)
            model_out = model(input_ids,attention_mask,token_type_ids,labels=label)
            val_loss += model_out.loss
            pred = model_out.logits.detach().cpu().numpy()
            f1 += flat_accuracy(pred,label.detach().cpu().numpy())
        f1_mean = f1 / size
        val_loss = val_loss / size
    print(f"F1:{(f1_mean):>4f},Avg loss: {val_loss:>8f} \n")
    return f1_mean
def run_model(epochs):
    best_f1 = 0
    for _ in range(epochs):
        train(train_dataloader,model,optimizer)
        f1_mean = evaluate(dev_dataloader,model)
        if best_f1 < f1_mean:
            best_f1 = f1_mean
            torch.save(model.state_dict(), f=model_save_path)
        print(
            'valid:  f1: %.5f,  best f1: %.5f\n' %
            (f1_mean,best_f1)
            # 'valid:  f1: %.5f, precision: %.5f, recall: %.5f, best f1: %.5f\n' %
            # (f1, precision, recall, self.best_val_f1)
        )

if __name__ == '__main__':
    run_model(epochs)
    # evaluate()
    
