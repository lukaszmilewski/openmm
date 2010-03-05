#ifndef OPENMM_CUSTOMHBONDFORCEIMPL_H_
#define OPENMM_CUSTOMHBONDFORCEIMPL_H_

/* -------------------------------------------------------------------------- *
 *                                   OpenMM                                   *
 * -------------------------------------------------------------------------- *
 * This is part of the OpenMM molecular simulation toolkit originating from   *
 * Simbios, the NIH National Center for Physics-Based Simulation of           *
 * Biological Structures at Stanford, funded under the NIH Roadmap for        *
 * Medical Research, grant U54 GM072970. See https://simtk.org.               *
 *                                                                            *
 * Portions copyright (c) 2008-2010 Stanford University and the Authors.      *
 * Authors: Peter Eastman                                                     *
 * Contributors:                                                              *
 *                                                                            *
 * Permission is hereby granted, free of charge, to any person obtaining a    *
 * copy of this software and associated documentation files (the "Software"), *
 * to deal in the Software without restriction, including without limitation  *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
 * and/or sell copies of the Software, and to permit persons to whom the      *
 * Software is furnished to do so, subject to the following conditions:       *
 *                                                                            *
 * The above copyright notice and this permission notice shall be included in *
 * all copies or substantial portions of the Software.                        *
 *                                                                            *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    *
 * THE AUTHORS, CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    *
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR      *
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE  *
 * USE OR OTHER DEALINGS IN THE SOFTWARE.                                     *
 * -------------------------------------------------------------------------- */

#include "ForceImpl.h"
#include "openmm/CustomHbondForce.h"
#include "openmm/Kernel.h"
#include "lepton/ExpressionTreeNode.h"
#include "lepton/ParsedExpression.h"
#include <utility>
#include <map>
#include <string>

namespace OpenMM {

/**
 * This is the internal implementation of CustomNonbondedForce.
 */

class CustomHbondForceImpl : public ForceImpl {
public:
    CustomHbondForceImpl(CustomHbondForce& owner);
    ~CustomHbondForceImpl();
    void initialize(ContextImpl& context);
    CustomHbondForce& getOwner() {
        return owner;
    }
    void updateContextState(ContextImpl& context) {
        // This force field doesn't update the state directly.
    }
    void calcForces(ContextImpl& context);
    double calcEnergy(ContextImpl& context);
    std::map<std::string, double> getDefaultParameters();
    std::vector<std::string> getKernelNames();
    /**
     * This is a utility routine that parses the energy expression, identifies the angles and dihedrals
     * in it, and replaces them with variables.  The particle indices returned in the maps are defined
     * as follows: 0=a1, 1=a2, 2=a3, 3=d1, 4=d2, 5=d3.
     *
     * @param force     the CustomHbondForce to process
     * @param distances on exist, this will contain an entry for each distance used in the expression.  The key is the name
     *                  of the corresponding variable, and the value is the list of particle indices.
     * @param angles    on exist, this will contain an entry for each angle used in the expression.  The key is the name
     *                  of the corresponding variable, and the value is the list of particle indices.
     * @param dihedrals on exist, this will contain an entry for each dihedral used in the expression.  The key is the name
     *                  of the corresponding variable, and the value is the list of particle indices.
     * @return a Parsed expression for the energy
     */
    static Lepton::ParsedExpression prepareExpression(const CustomHbondForce& force, std::map<std::string, std::vector<int> >& distances,
            std::map<std::string, std::vector<int> >& angles, std::map<std::string, std::vector<int> >& dihedrals);
private:
    class FunctionPlaceholder;
    static Lepton::ExpressionTreeNode replaceFunctions(const Lepton::ExpressionTreeNode& node, std::map<std::string, int> atoms,
            std::map<std::string, std::vector<int> >& distances, std::map<std::string, std::vector<int> >& angles,
            std::map<std::string, std::vector<int> >& dihedrals);
    CustomHbondForce& owner;
    Kernel kernel;
};

} // namespace OpenMM

#endif /*OPENMM_CUSTOMHBONDFORCEIMPL_H_*/
