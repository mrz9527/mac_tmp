# # -*- coding: utf-8 -*-
#
#
# import requests
# from urllib.parse import quote
# from lxml import etree
#
# # from lxml import html
#
#
# headers = {
#     'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9',
#     'user-agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/12.1.2 Safari/605.1.15',
#     "Cookie": "BIDUPSID=742F9CFB25D67771038BCE67269068D2; PSTM=1618808151; __yjs_duid=1_e9707b8b63c0a2bd08a7f1d551351eb31618808175500; BD_UPN=123253; BAIDUID=197E0FD6DB2A5EAA55B70DF26740C85B:FG=1; H_WISE_SIDS=110085_127969_179347_184716_189755_190619_194085_194511_194519_196426_197241_197711_198264_199176_199569_201193_201538_202652_203267_203279_203310_203519_204122_204715_204859_204907_205218_205484_205909_206927_206930_207235_207571_207729_207830_207864_207889_208050_208114_208267_208343_208686_208721_208972_209395_209437_209512_209568_209749_209771_209847_209944_209981_210095_210164_210242_210356_210520_210642_210653_210669_210709_210737_210754_210890_210892_210894_210900_210906_211022_211158_211180_211242_211301_211333_211416_211441_211456_211580_211732_211757_211785; BDUSS=VWd08tNkhNTnd2WWxBTG1wdmdZY0ZwRH5oWDRZZnNkS1pPTlpZcGcyZH5DdGhpRVFBQUFBJCQAAAAAAAAAAAEAAAAcBF6VaV9sb3ZlX2ZhbWlseXMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH99sGJ~fbBiMF; BDUSS_BFESS=VWd08tNkhNTnd2WWxBTG1wdmdZY0ZwRH5oWDRZZnNkS1pPTlpZcGcyZH5DdGhpRVFBQUFBJCQAAAAAAAAAAAEAAAAcBF6VaV9sb3ZlX2ZhbWlseXMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAH99sGJ~fbBiMF; H_WISE_SIDS_BFESS=110085_127969_179347_184716_189755_190619_194085_194511_194519_196426_197241_197711_198264_199176_199569_201193_201538_202652_203267_203279_203310_203519_204122_204715_204859_204907_205218_205484_205909_206927_206930_207235_207571_207729_207830_207864_207889_208050_208114_208267_208343_208686_208721_208972_209395_209437_209512_209568_209749_209771_209847_209944_209981_210095_210164_210242_210356_210520_210642_210653_210669_210709_210737_210754_210890_210892_210894_210900_210906_211022_211158_211180_211242_211301_211333_211416_211441_211456_211580_211732_211757_211785; BDORZ=B490B5EBF6F3CD402E515D22BCDA1598; BDSFRCVID=d4AOJexroG0mKjbjqamkulnZoOGMFYQTDYLEOwXPsp3LGJLVgmRYEG0Pt_NNvcP-ox_CogKK0mOTHUkF_2uxOjjg8UtVJeC6EG0Ptf8g0M5; H_BDCLCKID_SF=JnCHoD-bfI83H48k-4QEbbQH-UnLqhQJ3eOZ04n-ah02qljvjP6t3-Lq5fvXXtR-W20j0h7m3UTdsq76Wh35K5tTQP6rLtJkbjv4KKJxbp5HenrhjMcY-qLUhUJiB5OLBan7-RvIXKohJh7FM4tW3J0ZyxomtfQxtNRJ0DnjtpChbC-lD6KMDTvyeU5eetjK2CntsJOOaCvvhPbOy4oWK441Dh5C25bB5Hn7-D_a0JrdDqvoD-Jc3M04X-o9-hvT-54e2p3FBUQjotQ6Qft20b0k-fJXQ63uBC6Pab7jWhk2Dq72yhoOQlRX5q79atTMfNTJ-qcH0KQpsIJM5-DWbT8IjHCJJ6DHJnAOoKvt-5rDHJTg5DTjhPrM0PjTWMT-MTryKKJ-0RAbjRQ2yM-KjfDHXMciB5OMBanRhlRNB-3iV-OxDUvnyxAZ-nrq-MQxtNRJKRjjafDM8q6p3fRobUPUDMc9LUvP22cdot5yBbc8eIna5hjkbfJBQttjQn3hfIkj2CKLtC0BhK-wjjRjhR3H2x7K2bO0ato2WbCQ5Pbr8pcN2b5oQT8DyU6aat5jQ6TtWnruah7n8qQX0qOUWfAkWGO2b47fajuDbDJC5b6Coq5jDh3MBUCN5xbTe4ROX27y0hvctn6cShnaMUjrDRLbXU6BK5vPbNcZ0l8K3l02V-bIe-t2XjQhDHt8JT08JR3aQ5rtKRTffjrnhPF3b50FXP6-hnjy3bRLabct2tjHDJvGh-TAhU41XtjkQl3RymJ42-39LPO2hpRjyxv4bPuwWtoxJpOJX2owLJOxHR7W_pOvbURvD-ug3-7PaM5dtjTO2bc_5KnlfMQ_bf--QfbQ0hOhqP-jBRIE3-oJqCDhMKt93j; ab_sr=1.0.1_MWM4MDY4Y2VlNGM5NGIzYzc2Y2E1MGFkMDk5ZjQzY2YzNzZmOGRjOWNmODQ3OTBiY2FiYWIwZTU2YzFjZTljZDE3ODY3MjdjYjE4MjhmZGZlM2I1Yjc4MmJlMmZmZTgwYzM0ZmM5Y2ZiOTFmZjYzMDM5MWI3NTgxNzEyMWEwYzYxYzYzMmZlOTRjZGU3YjUxNjdjM2Y4NzIwNDFlNzE5ZDM1YWNlMDkxZDc5ZjlmM2FhMDcwZjk4ZTA0MTU4NDA1; BAIDUID_BFESS=197E0FD6DB2A5EAA55B70DF26740C85B:FG=1; BDSFRCVID_BFESS=d4AOJexroG0mKjbjqamkulnZoOGMFYQTDYLEOwXPsp3LGJLVgmRYEG0Pt_NNvcP-ox_CogKK0mOTHUkF_2uxOjjg8UtVJeC6EG0Ptf8g0M5; H_BDCLCKID_SF_BFESS=JnCHoD-bfI83H48k-4QEbbQH-UnLqhQJ3eOZ04n-ah02qljvjP6t3-Lq5fvXXtR-W20j0h7m3UTdsq76Wh35K5tTQP6rLtJkbjv4KKJxbp5HenrhjMcY-qLUhUJiB5OLBan7-RvIXKohJh7FM4tW3J0ZyxomtfQxtNRJ0DnjtpChbC-lD6KMDTvyeU5eetjK2CntsJOOaCvvhPbOy4oWK441Dh5C25bB5Hn7-D_a0JrdDqvoD-Jc3M04X-o9-hvT-54e2p3FBUQjotQ6Qft20b0k-fJXQ63uBC6Pab7jWhk2Dq72yhoOQlRX5q79atTMfNTJ-qcH0KQpsIJM5-DWbT8IjHCJJ6DHJnAOoKvt-5rDHJTg5DTjhPrM0PjTWMT-MTryKKJ-0RAbjRQ2yM-KjfDHXMciB5OMBanRhlRNB-3iV-OxDUvnyxAZ-nrq-MQxtNRJKRjjafDM8q6p3fRobUPUDMc9LUvP22cdot5yBbc8eIna5hjkbfJBQttjQn3hfIkj2CKLtC0BhK-wjjRjhR3H2x7K2bO0ato2WbCQ5Pbr8pcN2b5oQT8DyU6aat5jQ6TtWnruah7n8qQX0qOUWfAkWGO2b47fajuDbDJC5b6Coq5jDh3MBUCN5xbTe4ROX27y0hvctn6cShnaMUjrDRLbXU6BK5vPbNcZ0l8K3l02V-bIe-t2XjQhDHt8JT08JR3aQ5rtKRTffjrnhPF3b50FXP6-hnjy3bRLabct2tjHDJvGh-TAhU41XtjkQl3RymJ42-39LPO2hpRjyxv4bPuwWtoxJpOJX2owLJOxHR7W_pOvbURvD-ug3-7PaM5dtjTO2bc_5KnlfMQ_bf--QfbQ0hOhqP-jBRIE3-oJqCDhMKt93j; BD_CK_SAM=1; PSINO=1; delPer=0; BA_HECTOR=840k2gah8h8585ag850007581hnogm41f; ZFY=meTeDq:BOV8DDDw3YNTd7AyqARjmlNCbb4p:Be6:AhT9lo:C; COOKIE_SESSION=2558_0_8_9_5_7_1_0_8_6_0_0_512_0_0_0_1669035781_0_1669087263%7C9%232685411_287_1664095492%7C9; H_PS_PSSID=37784_36549_37733_37770_37834_37628_37777_37820_37723_36802_37535_37717_37742_26350_22158; H_PS_645EC=e94bAhL9R0I%2Fb9mECRsHarJX2JAkbUTeyauA3FqeX1bHtaRybrZxoVCYLzc; baikeVisitId=91c5a7f1-6674-4716-a489-1a58a41d6af2; BDSVRTM=262"
# }
#
#
# # ????????????????????????
# def get_search_url(search_keyword: str):
#     douban_url = "https://search.douban.com/movie/subject_search?search_text="
#     return douban_url + quote(search_keyword)
#
#
# def parse_content(search_url):
#     request = requests.get(url=search_url, headers=headers)
#     # print(request.text)
#     html = etree.HTML(request.text)
#     movies = html.xpath('//*[@id="content"]/div/div[1]/ol/li')
#     for movie in movies:
#         title = movie.xpath('./div/div[2]/div[1]/a/span[1]/text()')[0]
#         actors = movie.xpath('./div/div[2]/div[2]/p[1]/text()')[0].replace("\n                            ", "")
#         print(title)
#         print(actors)
#
#     """
#     ?????????????????? (2015)
#     """
#     # print(title)
#     # print(name)
#
#
# # search_keyword = input("???????????????:")
# # search_url = get_search_url(search_keyword)
#
# search_url = "https://movie.douban.com/top250"
# parse_content(search_url)


##????????????????????????????????????????????????????????????VIP??????
##???????????????
##???????????????????????????
##??????????????????python????????????????????????????????????????????????????????????
##??????????????????????????????requests????????????????????????
from multiprocessing import Pool
import requests


##??????????????????
def demo(i):
    ##???????????????url,??????%3d?????????????????????????????????0????????????i????????????1???????????????????????????????????????1?????????????????????001
    url = "https://vip.okokbo.com/20180114/ArVcZXQd/1000kb/hls/phJ51837151%03d.ts" % i
    ##????????????????????????
    headers = {
        "user-agent": "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.221 Safari/537.36"}
    ##???????????????????????????
    req = requests.get(url, headers=headers)
    ##?????????????????????????????????mp4?????????????????????url??????????????????
    with open('./mp4/ {}'.format(url[-10:]), 'wb') as f:
        f.write(req.content)

##?????????????????????
if __name__ == '__main__':
    ##??????????????????????????????????????????????????????????????????????????????????????????
    demo(100)
    # pool = Pool(20)
    # ##?????????????????????
    # for i in range(100):
    #     pool.apply_async(demo, (i,))
    #
    # pool.close()
    # pool.join()