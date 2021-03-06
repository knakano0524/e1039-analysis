
#include <iostream>

using namespace std;

int Fun4All_G4_E1039(const int nEvents = 100)
{
	const int use_g4steps = 1;

	gSystem->Load("libfun4all");
	gSystem->Load("libg4detectors");
	gSystem->Load("libg4testbench");
	gSystem->Load("libg4eval");
	gSystem->Load("libtruth_eval.so");

	///////////////////////////////////////////
	// Make the Server
	//////////////////////////////////////////
	Fun4AllServer *se = Fun4AllServer::instance();
	se->Verbosity(1);
	
	// particle gun
	PHG4ParticleGun *gun = new PHG4ParticleGun("PGUN");
	//  gun->set_name("anti_proton");
	gun->set_name("proton");
	//  gun->set_name("mu-");
	//  gun->set_name("proton");
	gun->set_vtx(0, 0, -30);
	gun->set_mom(0, 0, 120);
	se->registerSubsystem(gun);

	// Fun4All G4 module
	PHG4Reco *g4Reco = new PHG4Reco();
	//g4Reco->G4Seed(123);
	// no magnetic field
	g4Reco->set_field(5.);
	// size of the world - every detector has to fit in here
	g4Reco->SetWorldSizeX(200);
	g4Reco->SetWorldSizeY(200);
	g4Reco->SetWorldSizeZ(200);
	// shape of our world - it is a tube
	g4Reco->SetWorldShape("G4BOX");
	// this is what our world is filled with
	g4Reco->SetWorldMaterial("G4_Galactic");
	// Geant4 Physics list to use
	g4Reco->SetPhysicsList("FTFP_BERT");

	// our block "detector", size is in cm
	double xsize = 200.;
	double ysize = 200.;
	double zsize = 400.;

	PHG4CylinderSubsystem * tube = 0;

	tube = new PHG4CylinderSubsystem("Coil", 0);
	tube->SuperDetector("Coil");
	tube->set_double_param("length", 22.7);
	tube->set_double_param("rot_x", 90.);
	tube->set_double_param("rot_y", 0.);
	tube->set_double_param("rot_z", 0.);
	tube->set_double_param("place_x", 0.);
	tube->set_double_param("place_y", (22.7+4.)/2);
	tube->set_double_param("place_z", 0.);
	tube->set_double_param("radius", 12./2);
	tube->set_double_param("thickness", (44.45-12.)/2);
	tube->set_string_param("material", "Coil");          // material of tube
	tube->set_int_param("lengthviarapidity", 0);
	tube->set_int_param("use_g4steps", use_g4steps);
	tube->SetActive(1);                                   // it is an active volume - save G4Hits
	g4Reco->registerSubsystem(tube);


	tube = new PHG4CylinderSubsystem("Coil", 1);
	tube->SuperDetector("Coil");
	tube->set_double_param("length", 22.7);
	tube->set_double_param("rot_x", 90.);
	tube->set_double_param("rot_y", 0.);
	tube->set_double_param("rot_z", 0.);
	tube->set_double_param("place_x", 0.);
	tube->set_double_param("place_y", -(22.7+4.)/2);
	tube->set_double_param("place_z", 0.);
	tube->set_double_param("radius", 12./2);
	tube->set_double_param("thickness", (44.45-12.)/2);
	tube->set_string_param("material", "Coil");          // material of tube
	tube->set_int_param("lengthviarapidity", 0);
	tube->set_int_param("use_g4steps", use_g4steps);
	tube->SetActive(1);                                   // it is an active volume - save G4Hits
	g4Reco->registerSubsystem(tube);

	tube = new PHG4CylinderSubsystem("Target", 0);
	tube->SuperDetector("Target");
	tube->set_double_param("length", 7.9);
	tube->set_double_param("rot_x", 0.);
	tube->set_double_param("rot_y", 0.);
	tube->set_double_param("rot_z", 0.);
	tube->set_double_param("place_x", 0.);
	tube->set_double_param("place_y", 0.);
	tube->set_double_param("place_z", 0.);
	tube->set_double_param("radius", 0.);
	tube->set_double_param("thickness", (2.)/2);
	tube->set_string_param("material", "Target");          // material of tube
	tube->set_int_param("lengthviarapidity", 0);
	tube->set_int_param("use_g4steps", use_g4steps);
	tube->SetActive(1);                                   // it is an active volume - save G4Hits
	g4Reco->registerSubsystem(tube);

	PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
	g4Reco->registerSubsystem(truth);

	se->registerSubsystem(g4Reco);

	TruthEval* eval = new TruthEval("TruthEval","eval.root");
	se->registerSubsystem(eval);

	///////////////////////////////////////////
	// Output
	///////////////////////////////////////////

	// save a comprehensive  evaluation file
	PHG4DSTReader *ana = new PHG4DSTReader(
			string("DSTReader.root"));
	ana->set_save_particle(true);
	ana->set_load_all_particle(false);
	ana->set_load_active_particle(true);
	ana->set_save_vertex(true);
	ana->AddNode("Coil");
	se->registerSubsystem(ana);

	// input - we need a dummy to drive the event loop
	Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
	se->registerInputManager(in);

	Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", "DST.root");
	se->registerOutputManager(out);

	// a quick evaluator to inspect on hit/particle/tower level

	if (nEvents > 0)
	{
		se->run(nEvents);

		PHGeomUtility::ExportGeomtry(se->topNode(),"geom.root");

		// finish job - close and save output files
		se->End();
		std::cout << "All done" << std::endl;

		// cleanup - delete the server and exit
		delete se;
		gSystem->Exit(0);
	}
	return;
}

PHG4ParticleGun *get_gun(const char *name = "PGUN")
{
	Fun4AllServer *se = Fun4AllServer::instance();
	PHG4ParticleGun *pgun = (PHG4ParticleGun *) se->getSubsysReco(name);
	return pgun;
}
