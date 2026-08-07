project('epics-normative-types', 'cpp', version: '6.0.2', license: 'EPICS')

epics_com_dep = dependency('epics-com')
epics_pv_data_dep = dependency('epics-pv-data')

headers = [
  'src/pv/nt.h',
  'src/pv/ntutils.h',
  'src/pv/ntid.h',
  'src/pv/ntfield.h',
  'src/pv/ntscalar.h',
  'src/pv/ntscalarArray.h',
  'src/pv/ntnameValue.h',
  'src/pv/nttable.h',
  'src/pv/ntmultiChannel.h',
  'src/pv/ntscalarMultiChannel.h',
  'src/pv/ntndarray.h',
  'src/pv/ntmatrix.h',
  'src/pv/ntenum.h',
  'src/pv/ntunion.h',
  'src/pv/ntaggregate.h',
  'src/pv/ntattribute.h',
  'src/pv/ntcontinuum.h',
  'src/pv/nthistogram.h',
  'src/pv/nturi.h',
  'src/pv/ntndarrayAttribute.h',
]

sources = [
  'src/ntutils.cpp',
  'src/ntid.cpp',
  'src/ntfield.cpp',
  'src/ntscalar.cpp',
  'src/ntscalarArray.cpp',
  'src/ntnameValue.cpp',
  'src/nttable.cpp',
  'src/ntmultiChannel.cpp',
  'src/ntscalarMultiChannel.cpp',
  'src/ntndarray.cpp',
  'src/ntmatrix.cpp',
  'src/ntenum.cpp',
  'src/ntunion.cpp',
  'src/ntaggregate.cpp',
  'src/ntattribute.cpp',
  'src/ntcontinuum.cpp',
  'src/nthistogram.cpp',
  'src/nturi.cpp',
  'src/ntndarrayAttribute.cpp',
]

include_directories = ['src']

libnt = both_libraries(
  'epics-nt',
  sources,
  include_directories: include_directories,
  dependencies: [epics_com_dep, epics_pv_data_dep],
  install: true,
)

epics_nt_dep = declare_dependency(
  include_directories: include_directories,
  link_with: libnt,
)

tests = [
  ['ntaggregate_test', 'test/ntaggregateTest.cpp'],
  ['ntattribute_test', 'test/ntattributeTest.cpp'],
  ['ntcontinuum_test', 'test/ntcontinuumTest.cpp'],
  ['ntenum_test', 'test/ntenumTest.cpp'],
  ['ntfield_test', 'test/ntfieldTest.cpp'],
  ['nthistogram_test', 'test/nthistogramTest.cpp'],
  ['ntmatrix_test', 'test/ntmatrixTest.cpp'],
  ['ntmulti_channel_test', 'test/ntmultiChannelTest.cpp'],
  ['ntname_value_test', 'test/ntnameValueTest.cpp'],
  ['ntndarray_attribute_test', 'test/ntndarrayAttributeTest.cpp'],
  ['ntndarray_test', 'test/ntndarrayTest.cpp'],
  ['ntscalar_array_test', 'test/ntscalarArrayTest.cpp'],
  ['ntscalar_multi_channel_test', 'test/ntscalarMultiChannelTest.cpp'],
  ['ntscalar_test', 'test/ntscalarTest.cpp'],
  ['nttable_test', 'test/nttableTest.cpp'],
  ['ntunion_test', 'test/ntunionTest.cpp'],
  ['ntutils_test', 'test/ntutilsTest.cpp'],
  ['validator_test', 'test/validatorTest.cpp'],
]

foreach t : tests
  exe = executable(
    t[0],
    t[1],
    dependencies: [epics_com_dep, epics_pv_data_dep, epics_nt_dep]
  )
  test(t[0], exe, is_parallel: true)
endforeach

pkgconfig = import('pkgconfig')
pkgconfig.generate(libnt)

install_headers(headers, subdir: 'pv')
