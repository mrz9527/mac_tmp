import requests # 发送请求
import json
import re
from tqdm import tqdm

# 伪装
headers = {
'Cookie': 'tvfe_boss_uuid=bb88930a5ac8406d; appuser=C66D886E307ADD14; iip=0; _txjk_whl_uuid_aa5wayli=55a33622e35c40e987c810022a8c40c6; pgv_pvid=6990680204; ptui_loginuin=1321228067; RK=Kj3JwrkEZn; ptcz=42d9e016607f032705abd9792c4348479e6108da38fd5426d9ecaeff1088aa19; fqm_pvqid=d77fc224-90eb-4654-befc-ab7b6d275fb4; psrf_qqopenid=4F37937E43ECA9EAB02F9E89BE1860E2; psrf_qqaccess_token=2B1977379A78742A0B826B173FB09E92; wxunionid=; tmeLoginType=2; psrf_access_token_expiresAt=1664978634; psrf_qqrefresh_token=03721D80236524B49062B95719F2F8B4; psrf_qqunionid=FAEE1B5B10434CF5562642FABE749AB9; wxrefresh_token=; wxopenid=; euin=oKoAoK-ANens7z**; pac_uid=1_321228067; pgv_info=ssid=s891146992; o_cookie=3421355804; _qpsvr_localtk=0.6833337248736548; lv_play_index=94; video_omgid=1d3f8d535d23a20df3fe7382671d115b; vversion_name=8.2.95; o_minduid=AgDt7KNfzhCQXlgcV1YHh7y9HHG5zOXl; LPSJturn=49; LVINturn=0; LPHLSturn=88; LDERturn=976; LPPBturn=836; LPDFturn=942; LZTturn=36',
'Host': 'vd6.l.qq.com',
'Origin': 'https://v.qq.com',
'Referer': 'https://v.qq.com/',
'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36',
}
# 你需要什么数据
data = '{"buid":"vinfoad","vinfoparam":"charge=0&otype=ojson&defnpayver=1&spau=1&spaudio=0&spwm=1&sphls=2&host=v.qq.com&refer=v.qq.com&ehost=https%3A%2F%2Fv.qq.com%2Fx%2Fcover%2Fmzc00200qesgi36%2Fq0043w89bw8.html&sphttps=1&encryptVer=8.1&cKey=1500E9D95F2758D7532C6A0E11781991506C98470DB2A6FE38DBE83E238A70CBEBC8651D3C8D06CB2CA76A5951733EFFB354303B7F78E55BA64E346A1FF944691B024AF8490DAB0F39A65362E03920C530A84451199AEE8AA5D85800CE22CFB1D1E542AB6AD7968204E09590BF649103DEB343318B8C333A01DC68BDFE5506F2F6EFBDF842971AC7B3C33E676C3AB00DC6979C089CE5B9D0157C1C872649124E014F0911E1AE219F6182016F0753BFC6A257F7703AEE2D0220875D435B9F973BDD4D2CFB28F95A139BDA6EDB4982C60742F56D8F459A298EDCD3A503DD3240C5&clip=4&guid=1d3f8d535d23a20df3fe7382671d115b&flowid=2e5fc831c7c52340163ed8aeb2e87bdf&platform=10201&sdtfrom=v1010&appVer=3.5.57&unid=&auth_from=&auth_ext=&vid=q0043w89bw8&defn=fhd&fhdswitch=0&dtype=3&spsrt=2&tm=1659015229&lang_code=0&logintoken=%7B%22access_token%22%3A%2275C3D1F2FFB4B3897DF78DB2CF27A207%22%2C%22appid%22%3A%22101483052%22%2C%22vusession%22%3A%22_-4frZa6uPQZiaca9DsdLA.N%22%2C%22openid%22%3A%2203A0BB50713BC1C977C0F256056D2E36%22%2C%22vuserid%22%3A%22115600983%22%2C%22video_guid%22%3A%221d3f8d535d23a20df3fe7382671d115b%22%2C%22main_login%22%3A%22qq%22%7D&spvvpay=1&spadseg=3&hevclv=0&spsfrhdr=0&spvideo=0&drm=40","adparam":"pf=in&pf_ex=pc&pu=1&pt=0&platform=10201&from=0&flowid=2e5fc831c7c52340163ed8aeb2e87bdf&guid=1d3f8d535d23a20df3fe7382671d115b&coverid=mzc00200qesgi36&vid=q0043w89bw8&chid=0&tpid=22&refer=&url=https%3A%2F%2Fv.qq.com%2Fx%2Fcover%2Fmzc00200qesgi36%2Fq0043w89bw8.html<=qq&opid=03A0BB50713BC1C977C0F256056D2E36&atkn=75C3D1F2FFB4B3897DF78DB2CF27A207&appid=101483052&uid=115600983&tkn=_-4frZa6uPQZiaca9DsdLA.N&rfid=6a114baf649178b948c1e04d857d1f99_1659015218&v=1.4.119&vptag=www_baidu_com&ad_type=LD%7CKB%7CPVL&live=0&appversion=1.5.4&ty=web&adaptor=1&dtype=1&resp_type=json"}'
# 1. 发送请求 访问到 有m3u8链接的 网站
url = 'https://vd6.l.qq.com/proxyhttp'
response = requests.post(url=url, headers=headers, data=data)
# 2. 获取数据
# <Response [200]>: 请求成功
json_data = response.json()
# 3. 解析数据
# 字符串 eval(): 去除字符串引号的
vinfo = json.loads(json_data['vinfo'])
m3u8 = vinfo['vl']['vi'][0]['ul']['m3u8']
m3u8 = re.sub('#E.*', '', m3u8)
ts_list = m3u8.split()
for ts in tqdm(ts_list):
    ts_url = 'https://apd-068027621e9db154d1513aa430f4bd34.v.smtcdns.com/vhqts.tc.qq.com/AqWFyP9ZQE3sR_8eQqVkUN1N3Y1i-wBcovpMsF4yQ-fs/svp_50112/6Z8LbqHnpGer8cKogKCE-QY0CrsQQ9apcCt8s2QZ9L9cQTxH6Y1aRRiF3_oRpsFnpsTIDLVFc5IwlLF5rpWrBaIXLOs1gcOrVlfyChrmJWeg94k-r37UoxuygxHlhQ1YaPy32bh--YcS_Wc7YbMvh-uO6ZLjhBbpgS-8flAgkz5r_fOeFgtDDw/' + ts
    # 4. 保存数据
    video_data = requests.get(url=ts_url).content
    with open('硬糖少女.mp4', mode='ab') as f:
        f.write(video_data)