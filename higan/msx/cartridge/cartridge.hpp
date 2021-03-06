struct Cartridge {
  Node::Port port;
  Node::Peripheral node;

  auto manifest() const -> string { return information.manifest; }
  auto name() const -> string { return information.name; }
  auto region() const -> string { return information.region; }

  //cartridge.cpp
  Cartridge(string_view name);
  auto load(Node::Object, Node::Object) -> void;
  auto unload() -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto save() -> void;
  auto power() -> void;

  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  struct Information {
    string manifest;
    string name;
    string region;
  } information;

  Memory::Readable<uint8> rom;
  Memory::Writable<uint8> ram;

  const string portName;
};

extern Cartridge cartridge;
extern Cartridge expansion;
