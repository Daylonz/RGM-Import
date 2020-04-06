#define MATERIAL_DIFFUSE_FACETED_TWOSIDED		0
#define MATERIAL_DIFFUSE_FACETED				1
#define MATERIAL_DIFFUSE_TWOSIDED				2
#define MATERIAL_DIFFUSE						3
#define MATERIAL_OPAQUE_FACETED_TWOSIDED		4
#define MATERIAL_OPAQUE_FACETED					5
#define MATERIAL_OPAQUE_TWOSIDED				6
#define MATERIAL_OPAQUE							7

class AuMaterial
{
public:
	std::string name;
	int materialFlag;
};
