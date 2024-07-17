#include <cxxtest/TestSuite.h>
#include "../src/bitmap.h"

class SampleTestSuite : public CxxTest::TestSuite
{
public:
    void testAddLowSeverityIncident()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident incident;
        incident.type = ENVIRONMENTAL_REGULATIONS;
        std::strcpy(incident.description, "Test incident");
        incident.severity = -1;
        addComplianceIncident(&system, incident);
        TS_ASSERT_EQUALS(system.numIncidents, 0);
    }
    void testAddDataPrivacyIncident()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident incident;
        incident.type = DATA_PRIVACY;
        std::strcpy(incident.description, "Personal data breach");
        incident.severity = 5;
        addComplianceIncident(&system, incident);
        TS_ASSERT_EQUALS(system.numIncidents, 1);
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.type, DATA_PRIVACY);
        TS_ASSERT(std::strcmp(system.incidents[0].dataPrivacyIncident.description, "Personal data breach") == 0);
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.severity, 5);
    }
    void testAddComplianceIncident_InvalidDescription()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident incident;
        incident.type = DATA_PRIVACY;
        strcpy(incident.description, "");
        incident.severity = 8;
        addComplianceIncident(&system, incident);
        TS_ASSERT_EQUALS(system.numIncidents, 0);
    }
    //////////////////////////////////////////////////////////////////////////////////
    void testCalculateAverageSeverityWithTwoDataPrivacyIncidents(void)
    {
        ComplianceManagementSystem system;
        system.numIncidents = 3;
        ComplianceIncident incident1, incident2;
        incident1.type = DATA_PRIVACY;
        std::strcpy(incident1.description, "Data privacy incident 1");
        incident1.severity = 7;
        system.incidents[0].dataPrivacyIncident = incident1;
        incident2.type = DATA_PRIVACY;
        std::strcpy(incident2.description, "Data privacy incident 2");
        incident2.severity = 8;
        system.incidents[1].financialRegulationsIncident = incident2;
        float result = calculateAverageSeverity(system);
        TS_ASSERT_EQUALS(result, 5.0);
    }
    void testCalculateAverageSeverity_noIncidents()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        float result = calculateAverageSeverity(system);
        TS_ASSERT_EQUALS(result, 0.0);
        TS_TRACE("testCalculateAverageSeverity_noIncidents");
    }
    void testCalculateAverageSeverity_MultipleIncidents()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 4;
        ComplianceIncident incident1 = {DATA_PRIVACY, "Data breach", 8};
        incident1.severity = 8;
        system.incidents[0].dataPrivacyIncident = incident1;
        ComplianceIncident incident2 = {FINANCIAL_REGULATIONS, "Fraudulent activity", 5};
        incident1.severity = 5;
        system.incidents[1].financialRegulationsIncident = incident2;
        ComplianceIncident incident3 = {EMPLOYMENT_LAWS, "Unsafe working conditions", 3};
        incident1.severity = 3;
        system.incidents[2].employmentLawsIncident = incident3;
        float result = calculateAverageSeverity(system);
        TS_ASSERT_EQUALS(result, 4.0);
    }
    ////////////////////////////////////////////////////////////////////
    void testRemoveComplianceIncidentsOfType3()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident incident1 = {DATA_PRIVACY, "Leaked user data", 7};
        system.incidents[0].dataPrivacyIncident = incident1;
        system.numIncidents++;
        ComplianceIncident incident2 = {FINANCIAL_REGULATIONS, "Misreported financials", 5};
        system.incidents[1].financialRegulationsIncident = incident2;
        system.numIncidents++;
        ComplianceIncident incident3 = {EMPLOYMENT_LAWS, "Discriminatory hiring practices", 9};
        system.incidents[2].employmentLawsIncident = incident3;
        system.numIncidents++;
        ComplianceIncident incident4 = {ENVIRONMENTAL_REGULATIONS, "Improper waste disposal", 4};
        system.incidents[3].environmentalRegulationsIncident = incident4;
        system.numIncidents++;
        int result = removeComplianceIncidentsOfType(&system, FINANCIAL_REGULATIONS);
        TS_ASSERT_EQUALS(result, 1);
        TS_ASSERT_EQUALS(system.numIncidents, 3);
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.type, DATA_PRIVACY);
        TS_ASSERT_EQUALS(system.incidents[1].employmentLawsIncident.type, EMPLOYMENT_LAWS);
        TS_ASSERT_EQUALS(system.incidents[2].environmentalRegulationsIncident.type, ENVIRONMENTAL_REGULATIONS);
    }
    void testRemoveComplianceIncidentsOfType4()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        int result = removeComplianceIncidentsOfType(&system, EMPLOYMENT_LAWS);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(system.numIncidents, 0);
    }
    void testRemoveComplianceIncidentsOfType5()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 4;
        ComplianceIncident incident1 = {DATA_PRIVACY, "Data breach", 8};
        ComplianceIncident incident2 = {FINANCIAL_REGULATIONS, "Embezzlement", 6};
        ComplianceIncident incident3 = {EMPLOYMENT_LAWS, "Harassment", 9};
        ComplianceIncident incident4 = {ENVIRONMENTAL_REGULATIONS, "Oil spill", 7};
        system.incidents[0].dataPrivacyIncident = incident1;
        system.incidents[1].financialRegulationsIncident = incident2;
        system.incidents[2].employmentLawsIncident = incident3;
        system.incidents[3].environmentalRegulationsIncident = incident4;
        int numRemoved = removeComplianceIncidentsOfType(&system, EMPLOYMENT_LAWS);
        TS_ASSERT_EQUALS(numRemoved, 1);
        TS_ASSERT_EQUALS(system.numIncidents, 3);
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.type, DATA_PRIVACY);
        TS_ASSERT_EQUALS(system.incidents[1].financialRegulationsIncident.type, FINANCIAL_REGULATIONS);
        TS_ASSERT_EQUALS(system.incidents[2].environmentalRegulationsIncident.type, ENVIRONMENTAL_REGULATIONS);
        numRemoved = removeComplianceIncidentsOfType(&system, EMPLOYMENT_LAWS);
        TS_ASSERT_EQUALS(numRemoved, 0);
        TS_ASSERT_EQUALS(system.numIncidents, 3);
    }
    ///////////////////////////////////////////////////////////////////////////////////////
    void testFindHighestSeverityIncidentSameSeverity()
    {
        ComplianceManagementSystem system = {
            {{{FINANCIAL_REGULATIONS, "Data breach in financial system", 8}},
             {{DATA_PRIVACY, "Unauthorized access to personal data", 6}},
             {{EMPLOYMENT_LAWS, "Discrimination in hiring practices", 4}},
             {{ENVIRONMENTAL_REGULATIONS, "Illegal dumping of hazardous waste", 8}}},
            4};
        ComplianceIncident expectedIncident = {FINANCIAL_REGULATIONS, "Data breach in financial system", 8};
        ComplianceIncident result = findHighestSeverityIncident(system);
        TS_ASSERT_EQUALS(result.type, expectedIncident.type);
        TS_ASSERT_EQUALS(std::strncmp(result.description, expectedIncident.description, 100), 0);
        TS_ASSERT_EQUALS(result.severity, expectedIncident.severity);
    }
    void testFindHighestSeverityIncident_emptySystem_returnsEmptyIncident()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident result = findHighestSeverityIncident(system);
        TS_ASSERT_EQUALS(result.type, DATA_PRIVACY);
        TS_ASSERT_EQUALS(std::strncmp(result.description, "No incidents in the system", 26), 0);
        TS_ASSERT_EQUALS(result.severity, 0);
    }
    void testFindHighestSeverityIncident_singleIncident_returnsSingleIncident()
    {
        ComplianceManagementSystem system = {{DATA_PRIVACY, "Data breach", 5}, 1};
        ComplianceIncident incident = {DATA_PRIVACY, "Data breach", 5};
        ComplianceIncident result = findHighestSeverityIncident(system);
        TS_ASSERT_EQUALS(result.type, DATA_PRIVACY);
        TS_ASSERT_EQUALS(std::strncmp(result.description, "Data breach", 12), 0);
        TS_ASSERT_EQUALS(result.severity, 5);
    }
    ////////////////////////////////////////////////////////////////////////////////
    void testUpdateComplianceIncidentSeverityMultipleIncidents()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 3;
        ComplianceIncident incident1;
        incident1.type = DATA_PRIVACY;
        std::strcpy(incident1.description, "Data breach");
        incident1.severity = 7;
        system.incidents[0].dataPrivacyIncident = incident1;
        ComplianceIncident incident2;
        incident2.type = FINANCIAL_REGULATIONS;
        std::strcpy(incident2.description, "Fraud");
        incident2.severity = 5;
        system.incidents[1].financialRegulationsIncident = incident2;
        ComplianceIncident incident3;
        incident3.type = ENVIRONMENTAL_REGULATIONS;
        std::strcpy(incident3.description, "Waste disposal");
        incident3.severity = 9;
        system.incidents[2].environmentalRegulationsIncident = incident3;
        int newSeverity = 8;
        int result = updateComplianceIncidentSeverity(&system, incident1, newSeverity);
        TS_ASSERT_EQUALS(result, 0);
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.severity, newSeverity);
        TS_ASSERT_EQUALS(system.incidents[1].financialRegulationsIncident.severity, incident2.severity);
        TS_ASSERT_EQUALS(system.incidents[2].environmentalRegulationsIncident.severity, incident3.severity);
    }
    void testUpdateComplianceIncidentSeverity1()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 2;
        ComplianceIncident incident1 = {DATA_PRIVACY, "Confidential data breach", 5};
        ComplianceIncident incident2 = {EMPLOYMENT_LAWS, "Confidential data breach", 3};
        system.incidents[0].dataPrivacyIncident = incident1;
        system.incidents[1].employmentLawsIncident = incident2;
        int result = updateComplianceIncidentSeverity(&system, incident1, 8);
        TS_ASSERT_EQUALS(result, 0);                                            
        TS_ASSERT_EQUALS(system.incidents[0].dataPrivacyIncident.severity, 8);    
        TS_ASSERT_EQUALS(system.incidents[1].employmentLawsIncident.severity, 3); 
        TS_ASSERT_EQUALS(system.numIncidents, 2);                                
    }
    void testUpdateComplianceIncidentSeverity2()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 1;
        ComplianceIncident incident1 = {FINANCIAL_REGULATIONS, "Fraudulent transactions", 6};
        system.incidents[0].financialRegulationsIncident = incident1;
        int result = updateComplianceIncidentSeverity(&system, incident1, 12);
        TS_ASSERT_EQUALS(result, 1);                                                    
        TS_ASSERT_EQUALS(system.incidents[0].financialRegulationsIncident.severity, 6); 
        TS_ASSERT_EQUALS(system.numIncidents, 1);                                       
    }
    void testUpdateComplianceIncidentSeverity3()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 2;
        ComplianceIncident incident1 = {ENVIRONMENTAL_REGULATIONS, "Chemical spill", 8};
        ComplianceIncident incident2 = {EMPLOYMENT_LAWS, "Chemical spill", 2};
        system.incidents[0].environmentalRegulationsIncident = incident1;
        system.incidents[1].employmentLawsIncident = incident2;
        ComplianceIncident incident3 = {DATA_PRIVACY, "Confidential data breach", 5};
        int result = updateComplianceIncidentSeverity(&system, incident3, 8);
        TS_ASSERT_EQUALS(result, -1);             
        TS_ASSERT_EQUALS(system.numIncidents, 2); 
    }
    //////////////////////////////////////////////////////////////////
    void testRemoveComplianceIncident1()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident incident1 = {DATA_PRIVACY, "Data Privacy incident 1", 2};
        ComplianceIncident incident2 = {FINANCIAL_REGULATIONS, "Financial Regulations incident 1", 3};
        ComplianceIncident incident3 = {DATA_PRIVACY, "Data Privacy incident 2", 1};
        ComplianceIncident incident4 = {EMPLOYMENT_LAWS, "Employment Laws incident 1", 5};
        system.incidents[0].dataPrivacyIncident = incident1;
        system.incidents[1].financialRegulationsIncident = incident2;
        system.incidents[2].dataPrivacyIncident = incident3;
        system.incidents[3].employmentLawsIncident = incident4;
        system.numIncidents = 4;
        removeComplianceIncident(&system, incident1);
        TS_ASSERT_EQUALS(system.numIncidents, 3);
        TS_ASSERT_EQUALS(memcmp(&system.incidents[0], &incident2, sizeof(ComplianceIncident)), 0);
        TS_ASSERT_EQUALS(memcmp(&system.incidents[1], &incident3, sizeof(ComplianceIncident)), 0);
        TS_ASSERT_EQUALS(memcmp(&system.incidents[2], &incident4, sizeof(ComplianceIncident)), 0);
    }

    void testRemoveComplianceIncident2()
    {
        ComplianceManagementSystem system;
        system.numIncidents = 0;
        ComplianceIncident incident1 = {DATA_PRIVACY, "Data Privacy incident 1", 2};
        ComplianceIncident incident2 = {FINANCIAL_REGULATIONS, "Financial Regulations incident 1", 3};
        ComplianceIncident incident3 = {DATA_PRIVACY, "Data Privacy incident 2", 1};
        ComplianceIncident incident4 = {EMPLOYMENT_LAWS, "Employment Laws incident 1", 5};
        system.incidents[0].dataPrivacyIncident = incident1;
        system.incidents[1].financialRegulationsIncident = incident2;
        system.incidents[2].dataPrivacyIncident = incident3;
        system.incidents[3].employmentLawsIncident = incident4;
        system.numIncidents = 4;
        ComplianceIncident incident5 = {ENVIRONMENTAL_REGULATIONS, "Environmental Regulations incident 1", 2};
        removeComplianceIncident(&system, incident5);
        TS_ASSERT_EQUALS(system.numIncidents, 4);
        TS_ASSERT_EQUALS(memcmp(&system.incidents[0], &incident1, sizeof(ComplianceIncident)), 0);
        TS_ASSERT_EQUALS(memcmp(&system.incidents[1], &incident2, sizeof(ComplianceIncident)), 0);
        TS_ASSERT_EQUALS(memcmp(&system.incidents[2], &incident3, sizeof(ComplianceIncident)), 0);
        TS_ASSERT_EQUALS(memcmp(&system.incidents[3], &incident4, sizeof(ComplianceIncident)), 0);
    }
    void testRemoveComplianceIncidentWithMultipleIncidentsOfSameType()
    {
        ComplianceManagementSystem system;
        ComplianceIncident incident1 = {DATA_PRIVACY, "Leaked customer data", 10};
        system.incidents[0].dataPrivacyIncident = incident1;
        ComplianceIncident incident2 = {FINANCIAL_REGULATIONS, "Improper accounting", 5};
        system.incidents[1].financialRegulationsIncident = incident2;
        ComplianceIncident incident3 = {EMPLOYMENT_LAWS, "Discrimination in hiring process", 7};
        system.incidents[2].employmentLawsIncident = incident3;
        system.numIncidents = 3;
        removeComplianceIncident(&system, incident1);
        TS_ASSERT(system.numIncidents == 2);
        TS_ASSERT(memcmp(&system.incidents[0].financialRegulationsIncident, &incident2, sizeof(ComplianceIncident)) == 0);
        TS_ASSERT(memcmp(&system.incidents[1].employmentLawsIncident, &incident3, sizeof(ComplianceIncident)) == 0);
    }
};