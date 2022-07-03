#define SINFL_IMPLEMENTATION
#define SDEFL_IMPLEMENTATION
#include "sdefl.h"
#include "sinfl.h"
#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>

#define COMPRESSION_QUALITY_DEFLATE  8
#define FREE_DATA delete[]

template <typename T>
T* compressData(const T* data, int size, int* compSize) {
    struct sdefl sdefl = { 0 };
    int bounds = sdefl_bound(size);
    T* compData = new T[bounds];
    *compSize = sdeflate(&sdefl, compData, data, size, COMPRESSION_QUALITY_DEFLATE);
    std::cout << "SYSTEM: Compress data: Original size: " << size << " -> Comp. size: " << *compSize << std::endl;
    return compData;
}

template <typename T>
T* decompressData(const T* compData, int compSize, int* size) {
    T* data = new T[64*1024*1024];
    int length = sinflate(data, 64, compData, compSize);
    *size = length;
    std::cout << "SYSTEM: Decompress data: Comp. size: " << compSize << " -> Original size: " << *size << std::endl;
    return data;
}

int main() {
    int size;
    int compSize;
    char* potato = "class Bool {}\n"
"class Fiber {}\n"
"class Fn {}\n"
"class Null {}\n"
"class Num {}\n"
"\n"
"class Sequence {\n"
"  all(f) {\n"
"    var result = true\n"
"    for (element in this) {\n"
"      result = f.call(element)\n"
"      if (!result) return result\n"
"    }\n"
"    return result\n"
"  }\n"
"\n"
"  any(f) {\n"
"    var result = false\n"
"    for (element in this) {\n"
"      result = f.call(element)\n"
"      if (result) return result\n"
"    }\n"
"    return result\n"
"  }\n"
"\n"
"  contains(element) {\n"
"    for (item in this) {\n"
"      if (element == item) return true\n"
"    }\n"
"    return false\n"
"  }\n"
"\n"
"  count {\n"
"    var result = 0\n"
"    for (element in this) {\n"
"      result = result + 1\n"
"    }\n"
"    return result\n"
"  }\n"
"\n"
"  count(f) {\n"
"    var result = 0\n"
"    for (element in this) {\n"
"      if (f.call(element)) result = result + 1\n"
"    }\n"
"    return result\n"
"  }\n"
"\n"
"  each(f) {\n"
"    for (element in this) {\n"
"      f.call(element)\n"
"    }\n"
"  }\n"
"\n"
"  isEmpty { iterate(null) ? false : true }\n"
"\n"
"  map(transformation) { MapSequence.new(this, transformation) }\n"
"\n"
"  skip(count) {\n"
"    if (!(count is Num) || !count.isInteger || count < 0) {\n"
"      Fiber.abort(\"Count must be a non-negative integer.\")\n"
"    }\n"
"\n"
"    return SkipSequence.new(this, count)\n"
"  }\n"
"\n"
"  take(count) {\n"
"    if (!(count is Num) || !count.isInteger || count < 0) {\n"
"      Fiber.abort(\"Count must be a non-negative integer.\")\n"
"    }\n"
"\n"
"    return TakeSequence.new(this, count)\n"
"  }\n"
"\n"
"  where(predicate) { WhereSequence.new(this, predicate) }\n"
"\n"
"  reduce(acc, f) {\n"
"    for (element in this) {\n"
"      acc = f.call(acc, element)\n"
"    }\n"
"    return acc\n"
"  }\n"
"\n"
"  reduce(f) {\n"
"    var iter = iterate(null)\n"
"    if (!iter) Fiber.abort(\"Can't reduce an empty sequence.\")\n"
"\n"
"    // Seed with the first element.\n"
"    var result = iteratorValue(iter)\n"
"    while (iter = iterate(iter)) {\n"
"      result = f.call(result, iteratorValue(iter))\n"
"    }\n"
"\n"
"    return result\n"
"  }\n"
"\n"
"  join() { join(\"\") }\n"
"\n"
"  join(sep) {\n"
"    var first = true\n"
"    var result = \"\"\n"
"\n"
"    for (element in this) {\n"
"      if (!first) result = result + sep\n"
"      first = false\n"
"      result = result + element.toString\n"
"    }\n"
"\n"
"    return result\n"
"  }\n"
"\n"
"  toList {\n"
"    var result = List.new()\n"
"    for (element in this) {\n"
"      result.add(element)\n"
"    }\n"
"    return result\n"
"  }\n"
"}\n"
"\n"
"class MapSequence is Sequence {\n"
"  construct new(sequence, fn) {\n"
"    _sequence = sequence\n"
"    _fn = fn\n"
"  }\n"
"\n"
"  iterate(iterator) { _sequence.iterate(iterator) }\n"
"  iteratorValue(iterator) { _fn.call(_sequence.iteratorValue(iterator)) }\n"
"}\n"
"\n"
"class SkipSequence is Sequence {\n"
"  construct new(sequence, count) {\n"
"    _sequence = sequence\n"
"    _count = count\n"
"  }\n"
"\n"
"  iterate(iterator) {\n"
"    if (iterator) {\n"
"      return _sequence.iterate(iterator)\n"
"    } else {\n"
"      iterator = _sequence.iterate(iterator)\n"
"      var count = _count\n"
"      while (count > 0 && iterator) {\n"
"        iterator = _sequence.iterate(iterator)\n"
"        count = count - 1\n"
"      }\n"
"      return iterator\n"
"    }\n"
"  }\n"
"\n"
"  iteratorValue(iterator) { _sequence.iteratorValue(iterator) }\n"
"}\n"
"\n"
"class TakeSequence is Sequence {\n"
"  construct new(sequence, count) {\n"
"    _sequence = sequence\n"
"    _count = count\n"
"  }\n"
"\n"
"  iterate(iterator) {\n"
"    if (!iterator) _taken = 1 else _taken = _taken + 1\n"
"    return _taken > _count ? null : _sequence.iterate(iterator)\n"
"  }\n"
"\n"
"  iteratorValue(iterator) { _sequence.iteratorValue(iterator) }\n"
"}\n"
"\n"
"class WhereSequence is Sequence {\n"
"  construct new(sequence, fn) {\n"
"    _sequence = sequence\n"
"    _fn = fn\n"
"  }\n"
"\n"
"  iterate(iterator) {\n"
"    while (iterator = _sequence.iterate(iterator)) {\n"
"      if (_fn.call(_sequence.iteratorValue(iterator))) break\n"
"    }\n"
"    return iterator\n"
"  }\n"
"\n"
"  iteratorValue(iterator) { _sequence.iteratorValue(iterator) }\n"
"}\n"
"\n"
"class String is Sequence {\n"
"  bytes { StringByteSequence.new(this) }\n"
"  codePoints { StringCodePointSequence.new(this) }\n"
"\n"
"  split(delimiter) {\n"
"    if (!(delimiter is String) || delimiter.isEmpty) {\n"
"      Fiber.abort(\"Delimiter must be a non-empty string.\")\n"
"    }\n"
"\n"
"    var result = []\n"
"\n"
"    var last = 0\n"
"    var index = 0\n"
"\n"
"    var delimSize = delimiter.byteCount_\n"
"    var size = byteCount_\n"
"\n"
"    while (last < size && (index = indexOf(delimiter, last)) != -1) {\n"
"      result.add(this[last...index])\n"
"      last = index + delimSize\n"
"    }\n"
"\n"
"    if (last < size) {\n"
"      result.add(this[last..-1])\n"
"    } else {\n"
"      result.add(\"\")\n"
"    }\n"
"    return result\n"
"  }\n"
"\n"
"  replace(from, to) {\n"
"    if (!(from is String) || from.isEmpty) {\n"
"      Fiber.abort(\"From must be a non-empty string.\")\n"
"    } else if (!(to is String)) {\n"
"      Fiber.abort(\"To must be a string.\")\n"
"    }\n"
"\n"
"    var result = \"\"\n"
"\n"
"    var last = 0\n"
"    var index = 0\n"
"\n"
"    var fromSize = from.byteCount_\n"
"    var size = byteCount_\n"
"\n"
"    while (last < size && (index = indexOf(from, last)) != -1) {\n"
"      result = result + this[last...index] + to\n"
"      last = index + fromSize\n"
"    }\n"
"\n"
"    if (last < size) result = result + this[last..-1]\n"
"\n"
"    return result\n"
"  }\n"
"\n"
"  trim() { trim_(\"\\t\\r\\n \", true, true) }\n"
"  trim(chars) { trim_(chars, true, true) }\n"
"  trimEnd() { trim_(\"\\t\\r\\n \", false, true) }\n"
"  trimEnd(chars) { trim_(chars, false, true) }\n"
"  trimStart() { trim_(\"\\t\\r\\n \", true, false) }\n"
"  trimStart(chars) { trim_(chars, true, false) }\n"
"\n"
"  trim_(chars, trimStart, trimEnd) {\n"
"    if (!(chars is String)) {\n"
"      Fiber.abort(\"Characters must be a string.\")\n"
"    }\n"
"\n"
"    var codePoints = chars.codePoints.toList\n"
"\n"
"    var start\n"
"    if (trimStart) {\n"
"      while (start = iterate(start)) {\n"
"        if (!codePoints.contains(codePointAt_(start))) break\n"
"      }\n"
"\n"
"      if (start == false) return \"\"\n"
"    } else {\n"
"      start = 0\n"
"    }\n"
"\n"
"    var end\n"
"    if (trimEnd) {\n"
"      end = byteCount_ - 1\n"
"      while (end >= start) {\n"
"        var codePoint = codePointAt_(end)\n"
"        if (codePoint != -1 && !codePoints.contains(codePoint)) break\n"
"        end = end - 1\n"
"      }\n"
"\n"
"      if (end < start) return \"\"\n"
"    } else {\n"
"      end = -1\n"
"    }\n"
"\n"
"    return this[start..end]\n"
"  }\n"
"\n"
"  *(count) {\n"
"    if (!(count is Num) || !count.isInteger || count < 0) {\n"
"      Fiber.abort(\"Count must be a non-negative integer.\")\n"
"    }\n"
"\n"
"    var result = \"\"\n"
"    for (i in 0...count) {\n"
"      result = result + this\n"
"    }\n"
"    return result\n"
"  }\n"
"}\n"
"\n"
"class StringByteSequence is Sequence {\n"
"  construct new(string) {\n"
"    _string = string\n"
"  }\n"
"\n"
"  [index] { _string.byteAt_(index) }\n"
"  iterate(iterator) { _string.iterateByte_(iterator) }\n"
"  iteratorValue(iterator) { _string.byteAt_(iterator) }\n"
"\n"
"  count { _string.byteCount_ }\n"
"}\n"
"\n"
"class StringCodePointSequence is Sequence {\n"
"  construct new(string) {\n"
"    _string = string\n"
"  }\n"
"\n"
"  [index] { _string.codePointAt_(index) }\n"
"  iterate(iterator) { _string.iterate(iterator) }\n"
"  iteratorValue(iterator) { _string.codePointAt_(iterator) }\n"
"\n"
"  count { _string.count }\n"
"}\n"
"\n"
"class List is Sequence {\n"
"  addAll(other) {\n"
"    for (element in other) {\n"
"      add(element)\n"
"    }\n"
"    return other\n"
"  }\n"
"\n"
"  sort() { sort {|low, high| low < high } }\n"
"\n"
"  sort(comparer) {\n"
"    if (!(comparer is Fn)) {\n"
"      Fiber.abort(\"Comparer must be a function.\")\n"
"    }\n"
"    quicksort_(0, count - 1, comparer)\n"
"    return this\n"
"  }\n"
"\n"
"  quicksort_(low, high, comparer) {\n"
"    if (low < high) {\n"
"      var p = partition_(low, high, comparer)\n"
"      quicksort_(low, p - 1, comparer)\n"
"      quicksort_(p + 1, high, comparer)\n"
"    }\n"
"  }\n"
"\n"
"  partition_(low, high, comparer) {\n"
"    var p = this[high]\n"
"    var i = low - 1\n"
"    for (j in low..(high-1)) {\n"
"      if (comparer.call(this[j], p)) {  \n"
"        i = i + 1\n"
"        var t = this[i]\n"
"        this[i] = this[j]\n"
"        this[j] = t\n"
"      }\n"
"    }\n"
"    var t = this[i+1]\n"
"    this[i+1] = this[high]\n"
"    this[high] = t\n"
"    return i+1\n"
"  }\n"
"\n"
"  toString { \"[%(join(\", \"))]\" }\n"
"\n"
"  +(other) {\n"
"    var result = this[0..-1]\n"
"    for (element in other) {\n"
"      result.add(element)\n"
"    }\n"
"    return result\n"
"  }\n"
"\n"
"  *(count) {\n"
"    if (!(count is Num) || !count.isInteger || count < 0) {\n"
"      Fiber.abort(\"Count must be a non-negative integer.\")\n"
"    }\n"
"\n"
"    var result = []\n"
"    for (i in 0...count) {\n"
"      result.addAll(this)\n"
"    }\n"
"    return result\n"
"  }\n"
"}\n"
"\n"
"class Map is Sequence {\n"
"  keys { MapKeySequence.new(this) }\n"
"  values { MapValueSequence.new(this) }\n"
"\n"
"  toString {\n"
"    var first = true\n"
"    var result = \"{\"\n"
"\n"
"    for (key in keys) {\n"
"      if (!first) result = result + \", \"\n"
"      first = false\n"
"      result = result + \"%(key): %(this[key])\"\n"
"    }\n"
"\n"
"    return result + \"}\"\n"
"  }\n"
"\n"
"  iteratorValue(iterator) {\n"
"    return MapEntry.new(\n"
"        keyIteratorValue_(iterator),\n"
"        valueIteratorValue_(iterator))\n"
"  }\n"
"}\n"
"\n"
"class MapEntry {\n"
"  construct new(key, value) {\n"
"    _key = key\n"
"    _value = value\n"
"  }\n"
"\n"
"  key { _key }\n"
"  value { _value }\n"
"\n"
"  toString { \"%(_key):%(_value)\" }\n"
"}\n"
"\n"
"class MapKeySequence is Sequence {\n"
"  construct new(map) {\n"
"    _map = map\n"
"  }\n"
"\n"
"  iterate(n) { _map.iterate(n) }\n"
"  iteratorValue(iterator) { _map.keyIteratorValue_(iterator) }\n"
"}\n"
"\n"
"class MapValueSequence is Sequence {\n"
"  construct new(map) {\n"
"    _map = map\n"
"  }\n"
"\n"
"  iterate(n) { _map.iterate(n) }\n"
"  iteratorValue(iterator) { _map.valueIteratorValue_(iterator) }\n"
"}\n"
"\n"
"class Range is Sequence {}\n"
"\n"
"class System {\n"
"  static print() {\n"
"    writeString_(\"\\n\")\n"
"  }\n"
"\n"
"  static print(obj) {\n"
"    writeObject_(obj)\n"
"    writeString_(\"\\n\")\n"
"    return obj\n"
"  }\n"
"\n"
"  static printAll(sequence) {\n"
"    for (object in sequence) writeObject_(object)\n"
"    writeString_(\"\\n\")\n"
"  }\n"
"\n"
"  static write(obj) {\n"
"    writeObject_(obj)\n"
"    return obj\n"
"  }\n"
"\n"
"  static writeAll(sequence) {\n"
"    for (object in sequence) writeObject_(object)\n"
"  }\n"
"\n"
"  static writeObject_(obj) {\n"
"    var string = obj.toString\n"
"    if (string is String) {\n"
"      writeString_(string)\n"
"    } else {\n"
"      writeString_(\"[invalid toString]\")\n"
"    }\n"
"  }\n"
"}\n"
"\n"
"class ClassAttributes {\n"
"  self { _attributes }\n"
"  methods { _methods }\n"
"  construct new(attributes, methods) {\n"
"    _attributes = attributes\n"
"    _methods = methods\n"
"  }\n"
"  toString { \"attributes:%(_attributes) methods:%(_methods)\" }\n"
"}\n";

    char* apple = compressData<char>(potato, 9631, &compSize);
    std::string potat = decompressData<char>(apple, compSize, &size);

    FREE_DATA apple;

    std::cout << potat;
}