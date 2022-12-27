# -*- coding: utf-8 -*-

import numpy as np
import tritonclient.http as httpclient

from my_evaluate import evaluate

server_url = "10.11.11.31:8000"
model_name = "my_onnx"
model_version = "1"


def parse_model_http(model_metadata, model_config):
    inputs = model_metadata['inputs']
    outputs = model_metadata['outputs']
    max_batch_size = model_config['max_batch_size']

    return inputs, outputs, max_batch_size


def requestGenerator(input_datas, inputs, outputs):
    # Set the input data
    ins = []
    for i in range(len(inputs)):
        ins.append(
            # httpclient.InferInput(inputs[i]['name'], inputs[i]['shape'], inputs[i]['datatype']))
            httpclient.InferInput(inputs[i]['name'], [1, 250], inputs[i]['datatype']))
        data = np.asarray(input_datas[i]).astype('int64')
        ins[i].set_data_from_numpy(data, binary_data=True)  # TODO: tritonclient.utils.InferenceServerException: got unexpected numpy array shape [1, 250], expected [-1, 250]
        # ins[i].set_data_from_numpy(data.reshape(-1, 250), binary_data=True)

    outs = []
    out = httpclient.InferRequestedOutput(outputs[0]['name'], binary_data=True, class_count=1)
    outs.append(out)

    return ins, outs

def postprocess(response, output_name):
    output_array = response.as_numpy(output_name)
    for results in output_array:
        for result in results:
            cls = result.split(b':')
            if cls[1] == b'1':
                print("敏感")
            else:
                print("不敏感")

def client_main(input_datas):
    # httpclient
    triton_client = httpclient.InferenceServerClient(
        url=server_url, verbose=False, concurrency=1)

    # model_metadata
    model_metadata = triton_client.get_model_metadata(
        model_name=model_name, model_version=model_version)

    # model_config
    model_config = triton_client.get_model_config(
        model_name=model_name, model_version=model_version)

    # parse http
    inputs, outputs, max_batch_size = parse_model_http(
        model_metadata, model_config)

    responses = []

    # Send request
    ins, outs = requestGenerator(input_datas, inputs, outputs)
    sent_count = 1

    responses.append(
        triton_client.infer(model_name,
                            ins,
                            request_id=str(sent_count),
                            model_version=model_version,
                            outputs=outs))

    for response in responses:
        postprocess(response, outputs[0]['name'])

if __name__ == '__main__':
    while True:
        sentence = input("输入一段话，判断是否敏感:")
        input_datas = evaluate(sentence)
        client_main(input_datas)