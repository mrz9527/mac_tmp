import torch
from transformers import AlbertForSequenceClassification, BertTokenizerFast

model_path = "clue/albert_chinese_tiny"
tokenizer = BertTokenizerFast.from_pretrained(model_path, do_lower_case=True)

# model_save_path = './save_model/senstive_al.pth'
# model = AlbertForSequenceClassification.from_pretrained(model_path, num_labels=2)
# model.load_state_dict(torch.load(model_save_path, map_location=torch.device('cpu')))
device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
maxlen = 250


def evaluate(sentence):
    # model.to(device)
    # model.eval()
    data = tokenizer(sentence, return_offsets_mapping=True, max_length=maxlen, truncation=True, padding='max_length',
                     return_tensors='pt')
    with torch.no_grad():
        input_ids = data['input_ids'].to(device)
        attention_mask = data['attention_mask'].to(device)
        token_type_ids = data['token_type_ids'].to(device)
        input_datas = [token_type_ids, attention_mask, input_ids]
    return input_datas