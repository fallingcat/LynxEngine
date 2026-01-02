
bool IsAMDVLIW(unsigned int uVendorID, unsigned int uDeviceID)
{
	static unsigned int VLIWDeviceIDList[] = {
		0x9519, //{?AMD FireStream 9170  RV630 
		0x9452, //{?AMD FireStream 9250  RV770 
		0x9450, //{?AMD FireStream 9270  RV770 
		0x688D, //{?AMD FireStream 9350  Evergreen 
		0x688C, //{?AMD FireStream 9370  Evergreen 
		0x9598, //{?ATI All-in-Wonder HD  RV630 
		0x94C8, //{?ATI Mobility Radeon HD 2400 XT  M72 
		0x94C9, //{?ATI Mobility Radeon HD 2400  M72 
		0x9583, //{?ATI Mobility Radeon HD 2600 XT  M76 
		0x9581, //{?ATI Mobility Radeon HD 2600  M76 
		0x95C4, //{?ATI Mobility Radeon HD 3400 Series  M72 
		0x95C2, //{?ATI Mobility Radeon HD 3430  M72 
		0x9591, //{?ATI Mobility Radeon HD 3650  M76 
		0x9593, //{?ATI Mobility Radeon HD 3670  M76 
		0x9506, //{?ATI Mobility Radeon HD 3850 X2  M76 
		0x9504, //{?ATI Mobility Radeon HD 3850  M76 
		0x9509, //{?ATI Mobility Radeon HD 3870 X2  M76 
		0x9508, //{?ATI Mobility Radeon HD 3870  M76 
		0x9713, //{?ATI Mobility Radeon 4100  RS880M 
		0x9712, //{?ATI Mobility Radeon HD 4200 Series  RS880M 
		0x9552, //{?ATI Mobility Radeon HD 4300 Series  M92 
		0x9555, //{?ATI Mobility Radeon HD 4300/4500 Series  M92 
		0x9553, //{?ATI Mobility Radeon HD 4500/5100 Series  M92 
		0x9480, //{?ATI Mobility Radeon HD 4650  M96 
		0x9488, //{?ATI Mobility Radeon HD 4670  M96 
		0x94A0, //{?ATI Mobility Radeon HD 4830  M97 
		0x944A, //{?ATI Mobility Radeon HD 4850  M98 
		0x94A1, //{?ATI Mobility Radeon HD 4860  M97 
		0x945A, //{?ATI Mobility Radeon HD 4870  M98 
		0x68E1, //{?ATI Mobility Radeon HD 5000 Series  ManhattanP 
		0x68E0, //{?ATI Mobility Radeon HD 5000 Series  ManhattanP 
		0x68E0, //{?ATI Mobility Radeon HD 5400 Series  ManhattanP 
		0x68E1, //{?ATI Mobility Radeon HD 5400 Series  ManhattanP 
		0x94C3, //{?ATI Radeon HD 2350 Series  RV610 
		0x94C7, //{?ATI Radeon HD 2350  RV610 
		0x94C3, //{?ATI Radeon HD 2400 LE  RV610 
		0x94C1, //{?ATI Radeon HD 2400 Pro  RV610 
		0x94C3, //{?ATI Radeon HD 2400 PRO  RV610 
		0x94C3, //{?ATI Radeon HD 2400 Series  RV610 
		0x94C1, //{?ATI Radeon HD 2400 Series  RV610 
		0x94CC, //{?ATI Radeon HD 2400 Series  RV610 
		0x94C1, //{?ATI Radeon HD 2400 XT  RV610 
		0x94C3, //{?ATI Radeon HD 2400  RV610 
		0x9589, //{?ATI Radeon HD 2600 PRO  RV630 
		0x958A, //{?ATI Radeon HD 2600 X2 Series  RV630 
		0x9588, //{?ATI Radeon HD 2600 XT  RV630 
		0x9405, //{?ATI Radeon HD 2900 GT  R600 
		0x9403, //{?ATI Radeon HD 2900 PRO  R600 
		0x9400, //{?ATI Radeon HD 2900 PRO  R600 
		0x9400, //{?ATI Radeon HD 2900 XT  R600 
		0x9616, //{?ATI Radeon 3000 Graphics  RS780 
		0x9613, //{?ATI Radeon 3100 Graphics  RS780M 
		0x9611, //{?ATI Radeon 3100 Graphics  RS780 
		0x9612, //{?ATI Radeon HD 3200 Graphics  RS780M 
		0x9610, //{?ATI Radeon HD 3200 Graphics  RS780 
		0x9614, //{?ATI Radeon HD 3300 Graphics  RS780D 
		0x95C0, //{?ATI Radeon HD 3400 Series  RV610 
		0x95C5, //{?ATI Radeon HD 3400 Series  RV610 
		0x94C3, //{?ATI Radeon HD 3410  RV610 
		0x95C5, //{?ATI Radeon HD 3450  RV610 
		0x95C0, //{?ATI Radeon HD 3550  RV610 
		0x95C5, //{?ATI Radeon HD 3550  RV610 
		0x95C0, //{?ATI Radeon HD 3570  RV610 
		0x9598, //{?ATI Radeon HD 3600 Series  RV630 
		0x9591, //{?ATI Radeon HD 3600 Series  M76 
		0x9583, //{?ATI Radeon HD 3600 Series  M76 
		0x9581, //{?ATI Radeon HD 3600 Series  M76 
		0x9589, //{?ATI Radeon HD 3610  RV630 
		0x9505, //{?ATI Radeon HD 3690  RV630 
		0x9501, //{?ATI Radeon HD 3690  RV630 
		0x9598, //{?ATI Radeon HD3750  RV630 
		0x9598, //{?ATI Radeon HD 3730  RV630 
		0x9505, //{?ATI Radeon HD 3800 Series  RV630 
		0x9501, //{?ATI Radeon HD 3800 Series  RV630 
		0x9507, //{?ATI Radeon HD 3830  RV630 
		0x9513, //{?ATI Radeon HD 3850 X2  RV630 
		0x9505, //{?ATI Radeon HD 3850  RV630 
		0x950F, //{?ATI Radeon HD 3870 X2  RV630 
		0x9501, //{?ATI Radeon HD 3870  RV630 
		0x9710, //{?ATI Radeon HD 4200  RS880 
		0x95C5, //{?ATI Radeon HD 4230  RV610 
		0x9715, //{?ATI Radeon HD 4250  RS880 
		0x95C0, //{?ATI Radeon HD 4250  RV610 
		0x95C5, //{?ATI Radeon HD 4250  RV610 
		0x9714, //{?ATI Radeon HD 4290  RS880 
		0x95C5, //{?ATI Radeon HD 4300 Series  RV610 
		0x9555, //{?ATI Radeon HD4300/HD4500 series  M92 
		0x9552, //{?ATI Radeon HD 4300/4500 Series  M92 
		0x954F, //{?ATI Radeon HD 4300/4500 Series  R710 
		0x954F, //{?ATI Radeon HD 4450  R710 
		0x955F, //{?ATI Desktop Radeon HD 4500/4300 Series  M92 
		0x954F, //{?ATI Radeon HD 4520  R710 
		0x9555, //{?ATI Radeon HD 4550  M92 
		0x9540, //{?ATI Radeon HD 4550  R710 
		0x954F, //{?ATI Radeon HD 4550  R710 
		0x9598, //{?ATI Radeon HD 4570  RV630 
		0x9598, //{?ATI Radeon HD 4580  RV630 
		0x9540, //{?ATI Radeon HD 4590  R710 
		0x9498, //{?ATI Radeon HD 4600 Series  R730 
		0x9490, //{?ATI Radeon HD 4600 Series  R730 
		0x9598, //{?ATI Radeon HD 4610  RV630 
		0x9498, //{?ATI Radeon HD 4650  R730 
		0x9498, //{?ATI Radeon HD 4670  R730 
		0x94B4, //{?ATI Radeon HD 4700 Series  RV730 
		0x944E, //{?ATI Radeon HD 4700 Series  RV740 
		0x9498, //{?ATI Radeon HD 4700  R730 
		0x9490, //{?ATI Radeon HD 4710  R730 
		0x9498, //{?ATI Radeon HD 4720  R730 
		0x944E, //{?ATI Radeon HD 4730 Series  RV770 
		0x9505, //{?ATI Radeon HD 4730  RV630 
		0x9505, //{?ATI Radeon HD 4750  RV630 
		0x9501, //{?ATI Radeon HD 4750  RV630 
		0x94B3, //{?ATI Radeon HD 4770  RV740 
		0x94B5, //{?ATI Radeon HD 4770  RV740 
		0x9462, //{?ATI Radeon HD 4800 Series  RV790 
		0x9460, //{?ATI Radeon HD 4800 Series  RV790 
		0x9442, //{?ATI Radeon HD 4800 Series  RV770 
		0x9440, //{?ATI Radeon HD 4800 Series  RV770 
		0x944A, //{?ATI Radeon HD 4800 Series  M9x8 
		0x944C, //{?ATI Radeon HD 4800 Series  RV770 
		0x944E, //{?ATI Radeon HD 4810 series  RV770 
		0x944E, //{?ATI Radeon HD 4820  RV770 
		0x944C, //{?ATI Radeon HD 4830  RV770 
		0x9443, //{?ATI Radeon HD 4850 X2  RV700 
		0x9442, //{?ATI Radeon HD 4850  RV770 
		0x9441, //{?ATI Radeon HD 4870 X2  RV700 
		0x68C1, //{?ATI  Radeon HD 5000 Series  Manhattan 
		0x68E0, //{?ATI  Radeon HD 5400 Series  ManhattanP 
		0x68E0, //{?AMD  Radeon HD 5400 Series  ManhattanP 
		0x68E1, //{?ATI Radeon HD 5400 Series  ManhattanP 
		0x68F9, //{?ATI Radeon HD 5450  EvergreenC 
		0x68F9, //{?ATI Radeon HD 5470  EvergreenC 
		0x68E1, //{?ATI Radeon HD 5470  ManhattanP 
		0x68F9, //{?ATI Radeon HD 5490  EvergreenC 
		0x675F, //{?AMD Radeon HD 5500 Series  NI 
		0x68DA, //{?ATI Radeon HD 5500 Series  Evergreen 
		0x68F9, //{?ATI Radeon HD 5530  EvergreenC 
		0x68C7, //{?ATI Mobility Radeon HD 5570  Manhattan 
		0x68D9, //{?ATI Radeon HD 5570  Evergreen 
		0x68B9, //{?ATI Radeon HD 5600/5700  Evergreen 
		0x68DA, //{?ATI Radeon HD 5630  Evergreen 
		0x68D9, //{?ATI Radeon HD 5630  Evergreen 
		0x68D8, //{?ATI Radeon HD 5670  Evergreen 
		0x68D8, //{?ATI Radeon HD 5690  Evergreen 
		0x68B8, //{?ATI Radeon HD 5700 Series  Evergreen 
		0x68BE, //{?ATI Radeon HD 5700 Series  Evergreen 
		0x68D8, //{?ATI Radeon HD 5730  Evergreen 
		0x68A0, //{?ATI Mobility Radeon HD 5800 Series  Manhattan 
		0x68A1, //{?ATI Mobility Radeon HD 5800 Series  Manhattan 
		0x6899, //{?ATI Radeon HD 5800 Series  Evergreen 
		0x6898, //{?ATI Radeon HD 5800 Series  Evergreen 
		0x689E, //{?ATI Radeon HD 5800 Series  Evergreen 
		0x6899, //{?ATI Radeon HD 5850X2  Evergreen 
		0x689C, //{?ATI Radeon HD 5900 Series  EvergreenH 
		0x9805, //{?AMD Radeon HD 6200 series Graphics  Wrestler 
		0x9804, //{?AMD Radeon HD 6200 series Graphics  Wrestler 
		0x68E1, //{?AMD Radeon HD 6230  ManhattanP 
		0x68F9, //{?ATI Radeon HD 6230  EvergreenC 
		0x68E1, //{?ATI Radeon HD 6230  ManhattanP 
		0x68F9, //{?ATI Radeon HD 6250  EvergreenC 
		0x68E1, //{?ATI Radeon HD 6250  ManhattanP 
		0x9805, //{?AMD Radeon HD 6250 Graphics  Wrestler 
		0x9803, //{?AMD Radeon HD 6250 Graphics  Wrestler 
		0x68F9, //{?ATI Radeon HD 6290  EvergreenC 
		0x9807, //{?AMD Radeon HD 6290 Graphics  Wrestler 
		0x9803, //{?AMD Radeon HD 6300 series Graphics  Wrestler 
		0x9802, //{?AMD Radeon HD 6300 series Graphics  Wrestler 
		0x68E5, //{?AMD Radeon HD 6300M Series  ManhattanP 
		0x68E4, //{?AMD Radeon HD 6300 Series  ManhattanP 
		0x9804, //{?AMD Radeon HD 6310 Graphics  Wrestler 
		0x9802, //{?AMD Radeon HD 6310 Graphics  Wrestler 
		0x9806, //{?AMD Radeon HD 6320 Graphics  Wrestler 
		0x68F9, //{?ATI Radeon HD 6350 Graphics  EvergreenC 
		0x68E1, //{?ATI  Radeon HD 6350  ManhattanP 
		0x68E1, //{?ATI Radeon HD 6350  ManhattanP 
		0x68F9, //{?AMD Radeon HD 6350  EvergreenC 
		0x9642, //{?AMD Radeon HD 6370  Sumo_Desktop 
		0x9643, //{?AMD Radeon HD 6380  Sumo_Mobile 
		0x68DA, //{?ATI Radeon HD 6390  Evergreen 
		0x6770, //{?AMD Radeon HD 6400 Series  NI 
		0x6760, //{?AMD Radeon HD 6400M Series  Vancouver 
		0x9645, //{?AMD Radeon HD 6410  Sumo_Desktop 
		0x9644, //{?AMD Radeon HD 6410  Sumo_Desktop 
		0x6761, //{?AMD Radeon HD 6430  Vancouver 
		0x6779, //{?AMD Radeon HD 6450  NI 
		0x6763, //{?AMD Radeon E6460  Vancouver 
		0x9649, //{?AMD Radeon HD 6480  Sumo_Mobile 
		0x9648, //{?AMD Radeon HD 6480  Sumo_Mobile 
		0x68DA, //{?ATI Radeon HD 6490  Evergreen 
		0x68C1, //{?AMD Radeon HD 6500M/5600/5700 Series  Manhattan 
		0x68D9, //{?ATI Radeon HD 6510  Evergreen 
		0x9647, //{?AMD Radeon HD 6520G  Sumo_Mobile 
		0x964A, //{?AMD Radeon HD 6530D  Sumo_Desktop 
		0x9640, //{?AMD Radeon HD 6550D  Sumo_Desktop 
		0x6759, //{?AMD Radeon HD 6570 Graphics  NI 
		0x68C0, //{?AMD Radeon HD 6570M/5700 Series  Manhattan 
		0x6741, //{?AMD Radeon 6600M and 6700M Series  VancouverA 
		0x6750, //{?AMD Radeon HD 6600A Series  NI 
		0x68D9, //{?ATI Radeon HD 6610  Evergreen 
		0x9641, //{?AMD Radeon HD 6620  Sumo_Mobile 
		0x6742, //{?AMD Radeon HD 6625 Graphics  Vancouver 
		0x6758, //{?AMD Radeon HD 6670  NI 
		0x68B8, //{?AMD Radeon HD 6700 Series  Evergreen 
		0x68BA, //{?AMD Radeon HD 6700 Series  Evergreen 
		0x68BF, //{?AMD Radeon HD 6700 Series  Evergreen 
		0x673E, //{?AMD Radeon HD 6700 Series  NI 
		0x6740, //{?AMD Radeon HD 6700M Series  VancouverA 
		0x68BE, //{?ATI Radeon HD 6750  Evergreen 
		0x68BE, //{?AMD Radeon HD 6750 Graphics  Evergreen 
		0x68BF, //{?AMD Radeon HD 6750  Evergreen 
		0x6739, //{?AMD Radeon HD 6800 Series  NI 
		0x6738, //{?AMD Radeon HD 6800 Series  NI 
		0x689B, //{?AMD Radeon HD 6800 Series  Evergreen 
		0x68A8, //{?AMD Radeon HD 6800M Series  Manhattan 
		0x6739, //{?AMD Radeon HD 6850 X2  NI 
		0x6738, //{?AMD Radeon HD 6870 X2  NI 
		0x6719, //{?AMD Radeon HD 6900 Series  NICayman 
		0x6718, //{?AMD Radeon HD 6900 Series  NICayman 
		0x671D, //{?AMD Radeon HD 6900 Series  NICayman 
		0x671F, //{?AMD Radeon HD 6900 Series  NICayman 
		0x6720, //{?AMD Radeon HD 6900M Series  Vancouver 
		0x68FA, //{?AMD Radeon HD 7350  Evergreen 
	};

	if (uVendorID != 0x1002)
		return false;

	for (unsigned int i=0; i<sizeof(VLIWDeviceIDList)/sizeof(unsigned int); i++)
	{
		if (uDeviceID == VLIWDeviceIDList[i])
			return true;
	}

	return false;
}