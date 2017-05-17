import os
from jinja2 import Environment, FileSystemLoader, StrictUndefined


templates_path = os.path.dirname(os.path.realpath(__file__))
header_path = os.path.join(os.path.dirname(templates_path),
                           'include', 'dataframe')
src_path = os.path.join(os.path.dirname(templates_path), 'src')


template_env = Environment(loader=FileSystemLoader([templates_path]),
                           undefined=StrictUndefined,
                           trim_blocks=True,
                           lstrip_blocks=True)


def generate(path, **kwargs):
    t = template_env.get_template(path)
    return t.render(**kwargs)


def write_header(**kwargs):
    filename = os.path.join(header_path,
                            'column' + kwargs['name'] + '.h')
    t = generate('column.h', **kwargs)
    with open(filename, 'wt') as f:
        f.write(t)


def write_source(**kwargs):
    filename = os.path.join(src_path,
                            'column' + kwargs['name'] + '.c')
    t = generate('column.c', **kwargs)
    with open(filename, 'wt') as f:
        f.write(t)

write_header(name='Int8', type='int8_t')
write_header(name='Int16', type='int16_t')
write_header(name='Int32', type='int32_t')
write_header(name='Int64', type='int64_t')
write_header(name='UInt8', type='uint8_t')
write_header(name='UInt16', type='uint16_t')
write_header(name='UInt32', type='uint32_t')
write_header(name='UInt64', type='uint64_t')

write_source(name='Int8', type='int8_t')
write_source(name='Int16', type='int16_t')
write_source(name='Int32', type='int32_t')
write_source(name='Int64', type='int64_t')
write_source(name='UInt8', type='uint8_t')
write_source(name='UInt16', type='uint16_t')
write_source(name='UInt32', type='uint32_t')
write_source(name='UInt64', type='uint64_t')
