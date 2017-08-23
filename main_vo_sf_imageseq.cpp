/*********************************************************************************
**Fast Odometry and Scene Flow from RGB-D Cameras based on Geometric Clustering	**
**------------------------------------------------------------------------------**
**																				**
**	Copyright(c) 2017, Mariano Jaimez Tarifa, University of Malaga & TU Munich	**
**	Copyright(c) 2017, Christian Kerl, TU Munich								**
**	Copyright(c) 2017, MAPIR group, University of Malaga						**
**	Copyright(c) 2017, Computer Vision group, TU Munich							**
**																				**
**  This program is free software: you can redistribute it and/or modify		**
**  it under the terms of the GNU General Public License (version 3) as			**
**	published by the Free Software Foundation.									**
**																				**
**  This program is distributed in the hope that it will be useful, but			**
**	WITHOUT ANY WARRANTY; without even the implied warranty of					**
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the				**
**  GNU General Public License for more details.								**
**																				**
**  You should have received a copy of the GNU General Public License			**
**  along with this program. If not, see <http://www.gnu.org/licenses/>.		**
**																				**
*********************************************************************************/

#include <string.h>
#include <joint_vo_sf.h>


// -------------------------------------------------------------------------------
//								Instructions:
// You need to click on the window of the 3D Scene to be able to interact with it.
// 'n' - Load new frame and solve
// 's' - Turn on/off continuous estimation
// 'e' - Finish/exit
// -------------------------------------------------------------------------------

int main(int argc, char **argv)
{	
    const unsigned int res_factor = 2;
	VO_SF cf(res_factor);
    cf.setNumClusters(24);


	//Set first image to load, decimation factor and the sequence dir
	unsigned int im_count = 1;
	const unsigned int decimation = 1; //5
    std::string dir = "/home/tpatten/Code/Joint-VO-SF/data/opening_door/";
    if ( argc > 1 )
      dir = argv[1];

	//Load image and create pyramid
	cf.loadImageFromSequence(dir, im_count, res_factor);
	cf.createImagePyramid();

	//Create the 3D Scene
	cf.initializeSceneImageSeq();

	//Auxiliary variables
	int pushed_key = 0;
	bool continuous_exec = false, stop = false;
	
	while (!stop)
	{	
        if (cf.window.keyHit())
            pushed_key = cf.window.getPushedKey();
        else
            pushed_key = 0;

		switch (pushed_key) {

        //Load new image and solve
        case 'n':
			im_count += decimation;
			stop = cf.loadImageFromSequence(dir, im_count, res_factor);
            cf.run_VO_SF(true);
            cf.createImagesOfSegmentations();
            cf.updateSceneImageSeq();
            break;

		//Start/Stop continuous estimation
		case 's':
			continuous_exec = !continuous_exec;
			break;
			
		//Close the program
		case 'e':
			stop = true;
			break;
		}
	
		if ((continuous_exec)&&(!stop))
		{
			im_count += decimation;
			stop = cf.loadImageFromSequence(dir, im_count, res_factor);
            cf.run_VO_SF(true);
            cf.createImagesOfSegmentations();
			cf.updateSceneImageSeq();
		}
	}
    std::cin.ignore();

	return 0;
}

