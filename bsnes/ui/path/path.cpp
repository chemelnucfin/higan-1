#include "../base.hpp"
Path path;

string Path::home(const string &filename) {
  string path = { base, filename };
  if(file::exists(path)) return path;

  path = user;
  #if defined(PLATFORM_X) || defined(PLATFORM_OSX)
  path.append(".config/");
  mkdir(path, 0755);
  path.append("bsnes/");
  mkdir(path, 0755);
  #else
  path.append("bsnes/");
  mkdir(path, 0755);
  #endif
  path.append(filename);
  return path;
}

string Path::load(const string &path) {
  string value;

  if(path == "sfc") value = sfc;
  if(path == "bs") value = bs;
  if(path == "st") value = st;
  if(path == "gb") value = gb;

  if(path == "filter") value = filter;
  if(path == "shader") value = shader;

  if(value.beginswith("recent/")) value.ltrim<1>("recent/");
  if(value == "") value = base;
  return value;
}

void Path::save(const string &path, const string &value) {
  string *output = 0;

  if(path == "sfc") output = &sfc;
  if(path == "bs") output = &bs;
  if(path == "st") output = &st;
  if(path == "gb") output = &gb;

  if(path == "filter") output = &filter;
  if(path == "shader") output = &shader;

  if(output) {
    string &s = *output;
    if(s.beginswith("recent/") == false && s != "") return;
    s = { "recent/", value };
    return;
  }
}

string Path::load(SNES::Cartridge::Slot slot, const string &type, const string &suffix) {
  string romPath;
  switch(slot) {
    case SNES::Cartridge::Slot::Base: romPath = cartridge.baseName; break;
    case SNES::Cartridge::Slot::Bsx: romPath = cartridge.bsxName; break;
    case SNES::Cartridge::Slot::SufamiTurboA: romPath = cartridge.sufamiTurboAName; break;
    case SNES::Cartridge::Slot::SufamiTurboB: romPath = cartridge.sufamiTurboBName; break;
    case SNES::Cartridge::Slot::GameBoy: romPath = cartridge.gameBoyName; break;
  }

  string path = romPath;
  if(type == "srm" && srm != "") path = string(srm, notdir(path));
  if(type == "bsp" && srm != "") path = string(srm, notdir(path));
  if(type == "bss" && srm != "") path = string(srm, notdir(path));
  if(type == "sav" && srm != "") path = string(srm, notdir(path));
  if(type == "rtc" && rtc != "") path = string(rtc, notdir(path));
  if(type == "bsa" && bsa != "") path = string(bsa, notdir(path));
  if(type == "bst" && bst != "") path = string(bst, notdir(path));
  if(type == "cht" && cht != "") path = string(cht, notdir(path));
  if(type == "log" && log != "") path = string(log, notdir(path));

  if(path.beginswith("user/")) {
    path.ltrim<1>("user/");
    #if defined(PLATFORM_X) || defined(PLATFORM_OSX)
    path = string(user, ".config/bsnes/", path);
    #else
    path = string(user, "bsnes/", path);
    #endif
  } else if(path.beginswith("base/")) {
    path.ltrim<1>("base/");
    path = string(base, path);
  } else if(path.beginswith("./")) {
    path.ltrim<1>("./");
    path = string(dir(romPath), path);
  } else if(path.beginswith("../")) {
    string base = dir(romPath);
    base.rtrim<1>("/");
    path.ltrim<1>("../");
    path = string(dir(base), path);
  }

  return { path, suffix, ".", type };
}

void Path::load() {
  configuration::load(home("paths.cfg"));
}

void Path::save() {
  configuration::save(home("paths.cfg"));
}

Path::Path() {
  attach(sfc = "", "sfc");
  attach(bs = "", "bs");
  attach(st = "", "st");
  attach(gb = "", "gb");

  attach(filter = "", "filter");
  attach(shader = "", "shader");

  attach(satellaviewBios = "", "satellaviewBios");
  attach(sufamiTurboBios = "", "sufamiTurboBios");
  attach(superGameBoyBios = "", "superGameBoyBios");

  attach(srm = "", "srm");
  attach(rtc = "", "rtc");

  attach(bsa = "", "bsa");
  attach(bst = "", "bst");
  attach(cht = "", "cht");
  attach(log = "", "log");
}