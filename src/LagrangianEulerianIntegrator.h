/*
 * Copyright 2013 David Beckingsale.
 * 
 * This file is part of CleverLeaf.
 * 
 * CleverLeaf is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 * 
 * CleverLeaf is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * CleverLeaf. If not, see http://www.gnu.org/licenses/.
 */ 

#ifndef CLEVERLEAF_LAGRANGIANEULERIANINTEGRATOR_H_
#define CLEVERLEAF_LAGRANGIANEULERIANINTEGRATOR_H_

#include "LagrangianEulerianLevelIntegrator.h"

#include <vector>

#include "boost/shared_ptr.hpp"

#include "SAMRAI/mesh/GriddingAlgorithmStrategy.h"
#include "SAMRAI/hier/PatchHierarchy.h"
#include "SAMRAI/tbox/Database.h"
#include "SAMRAI/tbox/Timer.h"
#include "SAMRAI/tbox/StartupShutdownManager.h"

using namespace SAMRAI;

class LagrangianEulerianIntegrator {
  public:
    /**
     * Create a new LagriangianEulerianIntegrator.
     *
     * This object is responsible for advancing the simulation over the
     * adaptive mesh, as well as controlling regridding.
     *
     * @param input_db Input database containing setup parameters.
     * @param hierarchy The PatchHierarchy to use.
     * @param level_integrator The LagrangianEulerianLevelIntegrator object to
     *                         use.
     * @param gridding_algorithm The GriddingAlgorithm object to use.
     */
    LagrangianEulerianIntegrator(
        const boost::shared_ptr<tbox::Database>& input_db,
        const boost::shared_ptr<hier::PatchHierarchy>& hierarchy,
        const boost::shared_ptr<LagrangianEulerianLevelIntegrator>&
          level_integrator,
        const boost::shared_ptr<mesh::GriddingAlgorithmStrategy>& 
          gridding_algorithm);

    /**
     * Initialise the PatchHierarchy.
     *
     * @returns The initial dt.
     */
    double initializeHierarchy();

    /**
     * Advance the PatchHierarchy by the given dt.
     *
     * @param dt The dt to advance by.
     *
     * @returns The next safe dt.
     */
    double advanceHierarchy(const double dt);

    /**
     * Return the current simulation time.
     *
     * @returns The simulation time.
     */
    double getIntegratorTime() const { return d_integrator_time; }

    /**
     * Return the start time of the simulation.
     *
     * @returns The simulation start time.
     */
    double getStartTime() const { return d_start_time; }

    /**
     * Return the end time fo the simulation.
     *
     * @returns The simulation end time.
     */
    double getEndTime() const { return d_end_time; }

    /**
     * Return the current step.
     *
     * @returns The current step
     */
    int getIntegratorStep() const { return d_integrator_step; }

    /**
     * Check whether there are integration steps remaining.
     *
     * @returns True, if steps remaining.
     */
    bool stepsRemaining() const;

    const boost::shared_ptr<hier::PatchHierarchy> getPatchHierarchy() const
    {
      return d_patch_hierarchy;
    }

    boost::shared_ptr<LagrangianEulerianLevelIntegrator>
    getLevelIntegrator() const
    {
      return d_level_integrator;
    }

    boost::shared_ptr<mesh::GriddingAlgorithmStrategy> 
    getGriddingAlgorithm() const
    {
      return d_gridding_algorithm;
    }

    /**
     * Print out the field summary for the hierarchy.
     *
     * @returns The total kinetic energy, for testing.
     */
    double printFieldSummary();
  private:
    void initializeLevelData(const int level_number);

    void getMinHeirarchyDt(const bool initial_time);

    boost::shared_ptr<hier::PatchHierarchy> d_patch_hierarchy;
    boost::shared_ptr<LagrangianEulerianLevelIntegrator> d_level_integrator;
    boost::shared_ptr<mesh::GriddingAlgorithmStrategy> d_gridding_algorithm;

    double d_dt;
    double d_grow_dt;
    double d_max_dt;

    bool d_fix_dt;

    double d_start_time;
    double d_end_time;
    int d_end_step;

    double d_integrator_time;
    int d_integrator_step;

    int d_regrid_interval;
    std::vector<int> d_tag_buffer;

    static boost::shared_ptr<tbox::Timer> t_initialize_hierarchy;
    static boost::shared_ptr<tbox::Timer> t_advance_hierarchy;

    static void initializeCallback();
    static void finalizeCallback();

    static tbox::StartupShutdownManager::Handler s_initialize_handler;
};
#endif
