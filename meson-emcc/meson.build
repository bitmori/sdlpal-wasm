project('sdlpal-wasm', 'c', 'cpp', version: '1.0.0', default_options: ['cpp_std=c++11'])

cc = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')

# EDSDK 4.0.4 开始自带sdl3了 无需额外检测
# sdl_dep = dependency('sdl3', required: true)

# 使用构建时间戳作为版本号
datetime = run_command('date', ['+%Y%m%d%H%M%S'], check: true).stdout().strip()
generated_args = ['-DPAL_HAS_BUILD_TIMESTAMP="' + datetime + '"']

exported_fns = ['-Dfopen=EMSCRIPTEN_fopen', '-Dfclose=EMSCRIPTEN_fclose', '-Dmain=EMSCRIPTEN_main']

provided_options = [
#  '-DPAL_HAS_TOUCH='+(get_option('pal_has_touch')?'1':'0'),
]

# 通用 C/C++ 编译选项
emxx_args = [
  '-Wall', '-O2', '-gsource-map', '-fno-strict-aliasing',
  '-DUSE_SDL3=1', '-DPAL_HAS_PLATFORM_SPECIFIC_UTILS', 
  '-s', 'USE_SDL=3', '-s', 'USE_OGG=1', '-s', 'USE_VORBIS=1',
] + generated_args + provided_options

emcc_args = emxx_args + exported_fns

exported_fns_l = ['-s', 'EXPORTED_FUNCTIONS=["_EMSCRIPTEN_main"]']
# 链接选项
emlink_args = [
  '-gsource-map', '-O2',
  '-s', 'USE_SDL=3', '-s', 'USE_OGG=1', '-s', 'USE_VORBIS=1',
  '-s', 'EXPORTED_RUNTIME_METHODS=["cwrap"]',
  '-s', 'ASYNCIFY=1', '-s', 'EXIT_RUNTIME=1',
  '-s', 'MAX_WEBGL_VERSION=2', '-s', 'FULL_ES3', '-s', 'TOTAL_STACK=5242880',
  '--emrun',
  '-lidbfs.js'
] + exported_fns_l

# 设定头文件包含路径
inc = include_directories('src/emscripten', './src', 'src/sdl_compat', 'src/timidity')

# 收集源文件
sdl_compat_src = files(['src/sdl_compat/sdl_compat.c',])
adplug_c_src = files(['src/adplug/nuked_opl.c',])

adplug_cc_src = files([
  'src/adplug/fprovide.cpp',
  'src/adplug/binio.cpp',
  'src/adplug/rix.cpp',
  'src/adplug/dosbox_opls.cpp',
  'src/adplug/player.cpp',
  'src/adplug/emuopls.cpp',
  'src/adplug/surroundopl.cpp',
  'src/adplug/binfile.cpp',
  'src/adplug/mame_opls.cpp',
])

libmad_c_src = files([
  'src/libmad/fixed.c',
  'src/libmad/huffman.c',
  'src/libmad/bit.c',
  'src/libmad/frame.c',
  'src/libmad/synth.c',
  'src/libmad/stream.c',
  'src/libmad/layer3.c',
  'src/libmad/music_mad.c',
  'src/libmad/timer.c',
  'src/libmad/layer12.c',
  'src/libmad/decoder.c',
])

timidity_c_src = files([
  'src/timidity/instrum.c',
  'src/timidity/mix.c',
  'src/timidity/output.c',
  'src/timidity/tables.c',
  'src/timidity/timidity.c',
  'src/timidity/common.c',
  'src/timidity/playmidi.c',
  'src/timidity/readmidi.c',
  'src/timidity/resample.c',
])

sdlpal_c_src = files([
  'src/font.c',
  'src/resampler.c',
  'src/scene.c',
  'src/glslp.c',
  'src/midi_timidity.c',
  'src/opusplay.c',
  'src/yj1.c',
  'src/sound.c',
  'src/mp3play.c',
  'src/main.c',
  'src/audio.c',
  'src/palette.c',
  'src/input.c',
  'src/uibattle.c',
  'src/map.c',
  'src/game.c',
  'src/ending.c',
  'src/rngplay.c',
  'src/video_glsl.c',
  'src/ui.c',
  'src/battle.c',
  'src/midi_tsf.c',
  'src/script.c',
  'src/text.c',
  'src/aviplay.c',
  'src/mini_glloader.c',
  'src/global.c',
  'src/midi.c',
  'src/uigame.c',
  'src/itemmenu.c',
  'src/palcommon.c',
  'src/oggplay.c',
  'src/overlay.c',
  'src/video.c',
  'src/res.c',
  'src/fight.c',
  'src/play.c',
  'src/palcfg.c',
  'src/magicmenu.c',
  'src/util.c',
])
sdlpal_cc_src = files(['src/rixplay.cpp',])
emcc_extra_src = files('src/emscripten/emscripten_adapter.cpp')

all_sources = sdl_compat_src + adplug_c_src + adplug_cc_src + libmad_c_src + timidity_c_src + sdlpal_c_src + sdlpal_cc_src + emcc_extra_src

executable('sdlpal', all_sources, 
include_directories: inc,
cpp_args: emxx_args, c_args: emcc_args, link_args: emlink_args, 
override_options: ['b_staticpic=false'], # 防止静态链接报错
install: false
)
