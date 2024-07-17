#include <cxxtest/TestSuite.h>
#include "../src/bitmap.h"

class SampleTestSuite : public CxxTest::TestSuite
{
public:
    void testAddComplianceIncident_ValidIncident()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident incident;
        incident.type = DATA_PRIVACY;
        strcpy(incident.description, "Data breach occurred");
        incident.severity = 8;
        addComplianceIncident(&system, incident);
        TS_ASSERT_EQUALS(system.numIncidents, 1);
    }
    void testAddComplianceIncident_SystemFull()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 100;
        ComplianceIncident incident;
        incident.type = DATA_PRIVACY;
        strcpy(incident.description, "Data breach occurred");
        incident.severity = 8;
        addComplianceIncident(&system, incident);
        TS_ASSERT_EQUALS(system.numIncidents, 100);
    }

    void testAddComplianceIncident_InvalidType()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident incident;
        incident.type = (ComplianceType)5; // Invalid type
        strcpy(incident.description, "Data breach occurred");
        incident.severity = 8;
        addComplianceIncident(&system, incident);
        TS_ASSERT_EQUALS(system.numIncidents, 0);
    }
    ////////////////////////////////////////////////////////////
    void testCalculateAverageSeverity_oneIncident()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 1;
        ComplianceIncident incident;
        incident.type = DATA_PRIVACY;
        std::strcpy(incident.description, "Data breach");
        incident.severity = 10;
        system.incidents[0].dataPrivacyIncident = incident;
        float result = calculateAverageSeverity(system);
        TS_ASSERT_EQUALS(result, 10.0);
        TS_TRACE("testCalculateAverageSeverity_oneIncident");
    }

    void testCalculateAverageSeverity_multipleIncidents()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 3;
        ComplianceIncident incident1;
        incident1.type = DATA_PRIVACY;
        std::strcpy(incident1.description, "Data breach");
        incident1.severity = 10;
        system.incidents[0].dataPrivacyIncident = incident1;
        ComplianceIncident incident2;
        incident2.type = FINANCIAL_REGULATIONS;
        std::strcpy(incident2.description, "Fraud");
        incident2.severity = 5;
        system.incidents[1].financialRegulationsIncident = incident2;
        float result = calculateAverageSeverity(system);
        TS_ASSERT_EQUALS(result, 5.0);
        TS_TRACE("testCalculateAverageSeverity_multipleIncidents");
    }
    void testCalculateAverageSeverity_SingleIncident()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 2;
        ComplianceIncident incident = {DATA_PRIVACY, "Data breach", 8};
        system.incidents[0].dataPrivacyIncident = incident;
        float result = calculateAverageSeverity(system);
        TS_ASSERT_EQUALS(result, 4.0);
    }

    //////////////////////////////////////////////////////////
    void testRemoveComplianceIncidentsOfType_noMatchingType()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 3;
        ComplianceIncident incident1;
        incident1.type = DATA_PRIVACY;
        std::strcpy(incident1.description, "Data breach");
        incident1.severity = 10;
        system.incidents[0].dataPrivacyIncident = incident1;
        ComplianceIncident incident2;
        incident2.type = FINANCIAL_REGULATIONS;
        std::strcpy(incident2.description, "Fraud");
        incident2.severity = 5;
        system.incidents[1].financialRegulationsIncident = incident2;
        ComplianceIncident incident3;
        incident3.type = ENVIRONMENTAL_REGULATIONS;
        std::strcpy(incident3.description, "Oil spill");
        incident3.severity = 8;
        system.incidents[2].environmentalRegulationsIncident = incident3;
        int result = removeComplianceIncidentsOfType(&system, EMPLOYMENT_LAWS);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(system.numIncidents, 3);
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.severity, 10);
        TS_ASSERT_EQUALS(system.incidents[1].financialRegulationsIncident.severity, 5);
        TS_ASSERT_EQUALS(system.incidents[2].environmentalRegulationsIncident.severity, 8);
        TS_TRACE("testRemoveComplianceIncidentsOfType_noMatchingType");
    }
    void testRemoveComplianceIncidentsOfType1()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident incident1 = {DATA_PRIVACY, "Leaked user data", 7};
        system.incidents[0].dataPrivacyIncident = incident1;
        system.numIncidents++;
        int result = removeComplianceIncidentsOfType(&system, DATA_PRIVACY);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(system.numIncidents, 0);
    }
    void testRemoveComplianceIncidentsOfType2()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident incident1 = {DATA_PRIVACY, "Leaked user data", 7};
        system.incidents[0].dataPrivacyIncident = incident1;
        system.numIncidents++;
        ComplianceIncident incident2 = {FINANCIAL_REGULATIONS, "Misreported financials", 5};
        system.incidents[1].financialRegulationsIncident = incident2;
        system.numIncidents++;
        int result = removeComplianceIncidentsOfType(&system, EMPLOYMENT_LAWS);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(system.numIncidents, 2);
    }
    ///////////////////////////////////////////////////////////////
    void testFindHighestSeverityIncidentEmpty()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident emptyIncident = {DATA_PRIVACY, "No incidents in the system", 0};
        ComplianceIncident result = findHighestSeverityIncident(system);
        TS_ASSERT_EQUALS(result.type, emptyIncident.type);
        TS_ASSERT_EQUALS(std::strncmp(result.description, emptyIncident.description, 100), 0);
        TS_ASSERT_EQUALS(result.severity, emptyIncident.severity);
    }
    void testFindHighestSeverityIncidentSingle()
    {
        ComplianceManagementSystem system = {{{FINANCIAL_REGULATIONS, "Data breach in financial system", 8}}, 1};
        ComplianceIncident expectedIncident = {FINANCIAL_REGULATIONS, "Data breach in financial system", 8};
        ComplianceIncident result = findHighestSeverityIncident(system);
        TS_ASSERT_EQUALS(result.type, expectedIncident.type);
        TS_ASSERT_EQUALS(std::strncmp(result.description, expectedIncident.description, 100), 0);
        TS_ASSERT_EQUALS(result.severity, expectedIncident.severity);
    }

    // Test case for a system with multiple incidents
    void testFindHighestSeverityIncidentMultiple()
    {
        ComplianceManagementSystem system = {
            {{{FINANCIAL_REGULATIONS, "Data breach in financial system", 8}},
             {{DATA_PRIVACY, "Unauthorized access to personal data", 6}},
             {{EMPLOYMENT_LAWS, "Discrimination in hiring practices", 4}},
             {{ENVIRONMENTAL_REGULATIONS, "Illegal dumping of hazardous waste", 10}}},
            4};
        ComplianceIncident expectedIncident = {ENVIRONMENTAL_REGULATIONS, "Illegal dumping of hazardous waste", 10};
        ComplianceIncident result = findHighestSeverityIncident(system);
        TS_ASSERT_EQUALS(result.type, expectedIncident.type);
        TS_ASSERT_EQUALS(std::strncmp(result.description, expectedIncident.description, 100), 0);
        TS_ASSERT_EQUALS(result.severity, expectedIncident.severity);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////

    void testUpdateComplianceIncidentSeverityValidInput()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 1;
        ComplianceIncident incident;
        incident.type = DATA_PRIVACY;
        std::strcpy(incident.description, "Data breach");
        incident.severity = 7;
        system.incidents[0].dataPrivacyIncident = incident;
        int newSeverity = 8;
        int result = updateComplianceIncidentSeverity(&system, incident, newSeverity);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.severity, newSeverity);
    }
    void testUpdateComplianceIncidentSeverityInvalidSeverity()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 1;
        ComplianceIncident incident;
        incident.type = DATA_PRIVACY;
        std::strcpy(incident.description, "Data breach");
        incident.severity = 7;
        system.incidents[0].dataPrivacyIncident = incident;
        int newSeverity = 20;
        int result = updateComplianceIncidentSeverity(&system, incident, newSeverity);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.severity, incident.severity);
    }

    void testUpdateComplianceIncidentSeverityIncidentNotFound()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 1;
        ComplianceIncident incident;
        incident.type = DATA_PRIVACY;
        std::strcpy(incident.description, "Data breach");
        incident.severity = 7;
        int newSeverity = 8;
        int result = updateComplianceIncidentSeverity(&system, incident, newSeverity);
        TS_ASSERT_EQUALS(result, 0);
    }
    //////////////////////////////////////////////////////////////////////

    void testRemoveComplianceIncident_NotFound(void)
    {
        ComplianceManagementSystem system;
        system.numIncidents = 1;
        ComplianceIncidentUnion union1;
        union1.dataPrivacyIncident.type = DATA_PRIVACY;
        union1.dataPrivacyIncident.severity = 1;
        strcpy(union1.dataPrivacyIncident.description, "description1");
        system.incidents[0] = union1;
        ComplianceIncident incident;
        incident.type = FINANCIAL_REGULATIONS;
        incident.severity = 1;
        strcpy(incident.description, "description2");
        removeComplianceIncident(&system, incident);
        TS_ASSERT_EQUALS(system.numIncidents, 1);
    }
    void testRemoveComplianceIncident_Success(void)
    {
        ComplianceManagementSystem system;
        system.numIncidents = 2;
        ComplianceIncidentUnion union1;
        union1.dataPrivacyIncident.type = DATA_PRIVACY;
        union1.dataPrivacyIncident.severity = 1;
        strcpy(union1.dataPrivacyIncident.description, "description1");
        system.incidents[0] = union1;
        ComplianceIncidentUnion union2;
        union2.financialRegulationsIncident.type = FINANCIAL_REGULATIONS;
        union2.financialRegulationsIncident.severity = 2;
        strcpy(union2.financialRegulationsIncident.description, "description2");
        system.incidents[1] = union2;
        ComplianceIncident incident;
        incident.type = FINANCIAL_REGULATIONS;
        incident.severity = 2;
        strcpy(incident.description, "description2");
        removeComplianceIncident(&system, incident);
        TS_ASSERT_EQUALS(system.numIncidents, 2);
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.type, DATA_PRIVACY);
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.severity, 1);
        TS_ASSERT_EQUALS(strcmp(system.incidents[0].dataPrivacyIncident.description, "description1"), 0);
    }
    void testRemoveComplianceIncident_Empty(void)
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncidentUnion union1;
        union1.dataPrivacyIncident.type = DATA_PRIVACY;
        union1.dataPrivacyIncident.severity = 1;
        strcpy(union1.dataPrivacyIncident.description, "description1");
        removeComplianceIncident(&system, union1.dataPrivacyIncident);
        TS_ASSERT_EQUALS(system.numIncidents, 0);
    }
   
};