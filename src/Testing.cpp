#include "Testing.h"
#include "Buffer.h"

void DoTest()
{
	buffer::BufferLayout layout{};
	layout.AddField<bool>("bool");
	layout.AddField<bool>("bool1");
	layout.AddField<bool>("bool2");
	layout.AddField<float>("floaty");

	buffer::Buffer buff(layout);
	const auto& cbuf = buff;
	bool& val = cbuf["bool"];
	
}