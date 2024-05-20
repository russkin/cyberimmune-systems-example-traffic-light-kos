from bottle import run, get, post, request, route, response
from json import dumps

@route('/mode/<tl_id>', method = 'GET')
def process_test(tl_id):
    print(f"requested mode for TL with ID {tl_id}")
    mode = { "id": int(tl_id) }
    response.content_type = 'application/json'
    return dumps(mode)


run(host='0.0.0.0', port=8081, debug=True)
