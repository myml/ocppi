// Thish file is generated by /tools/codegen powered by quicktype.
// DO NOT EDIT IT.

// clang-format off

//  To parse this JSON data, first install
//
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     Generators.hpp data = nlohmann::json::parse(jsonString);

#pragma once

#include <optional>
#include <nlohmann/json.hpp>
#include <ocppi/runtime/features/types/helper.hpp>

#include <ocppi/runtime/features/types/Features.hpp>
#include <ocppi/runtime/features/types/Linux.hpp>
#include <ocppi/runtime/features/types/Selinux.hpp>
#include <ocppi/runtime/features/types/Seccomp.hpp>
#include <ocppi/runtime/features/types/SeccompOperators.hpp>
#include <ocppi/runtime/features/types/SeccompFlag.hpp>
#include <ocppi/runtime/features/types/SeccompArch.hpp>
#include <ocppi/runtime/features/types/SeccompAction.hpp>
#include <ocppi/runtime/features/types/NamespaceType.hpp>
#include <ocppi/runtime/features/types/IntelRdt.hpp>
#include <ocppi/runtime/features/types/Cgroup.hpp>
#include <ocppi/runtime/features/types/Apparmor.hpp>

namespace ocppi {
namespace runtime {
namespace features {
namespace types {
void from_json(const json & j, Apparmor & x);
void to_json(json & j, const Apparmor & x);

void from_json(const json & j, Cgroup & x);
void to_json(json & j, const Cgroup & x);

void from_json(const json & j, IntelRdt & x);
void to_json(json & j, const IntelRdt & x);

void from_json(const json & j, Seccomp & x);
void to_json(json & j, const Seccomp & x);

void from_json(const json & j, Selinux & x);
void to_json(json & j, const Selinux & x);

void from_json(const json & j, Linux & x);
void to_json(json & j, const Linux & x);

void from_json(const json & j, Features & x);
void to_json(json & j, const Features & x);

void from_json(const json & j, NamespaceType & x);
void to_json(json & j, const NamespaceType & x);

void from_json(const json & j, SeccompAction & x);
void to_json(json & j, const SeccompAction & x);

void from_json(const json & j, SeccompArch & x);
void to_json(json & j, const SeccompArch & x);

void from_json(const json & j, SeccompFlag & x);
void to_json(json & j, const SeccompFlag & x);

void from_json(const json & j, SeccompOperators & x);
void to_json(json & j, const SeccompOperators & x);

inline void from_json(const json & j, Apparmor& x) {
x.enabled = get_stack_optional<bool>(j, "enabled");
}

inline void to_json(json & j, const Apparmor & x) {
j = json::object();
if (x.enabled) {
j["enabled"] = x.enabled;
}
}

inline void from_json(const json & j, Cgroup& x) {
x.rdma = get_stack_optional<bool>(j, "rdma");
x.systemd = get_stack_optional<bool>(j, "systemd");
x.systemdUser = get_stack_optional<bool>(j, "systemdUser");
x.v1 = get_stack_optional<bool>(j, "v1");
x.v2 = get_stack_optional<bool>(j, "v2");
}

inline void to_json(json & j, const Cgroup & x) {
j = json::object();
if (x.rdma) {
j["rdma"] = x.rdma;
}
if (x.systemd) {
j["systemd"] = x.systemd;
}
if (x.systemdUser) {
j["systemdUser"] = x.systemdUser;
}
if (x.v1) {
j["v1"] = x.v1;
}
if (x.v2) {
j["v2"] = x.v2;
}
}

inline void from_json(const json & j, IntelRdt& x) {
x.enabled = get_stack_optional<bool>(j, "enabled");
}

inline void to_json(json & j, const IntelRdt & x) {
j = json::object();
if (x.enabled) {
j["enabled"] = x.enabled;
}
}

inline void from_json(const json & j, Seccomp& x) {
x.actions = get_stack_optional<std::vector<SeccompAction>>(j, "actions");
x.archs = get_stack_optional<std::vector<SeccompArch>>(j, "archs");
x.enabled = get_stack_optional<bool>(j, "enabled");
x.knownFlags = get_stack_optional<std::vector<SeccompFlag>>(j, "knownFlags");
x.operators = get_stack_optional<std::vector<SeccompOperators>>(j, "operators");
x.supportedFlags = get_stack_optional<std::vector<SeccompFlag>>(j, "supportedFlags");
}

inline void to_json(json & j, const Seccomp & x) {
j = json::object();
if (x.actions) {
j["actions"] = x.actions;
}
if (x.archs) {
j["archs"] = x.archs;
}
if (x.enabled) {
j["enabled"] = x.enabled;
}
if (x.knownFlags) {
j["knownFlags"] = x.knownFlags;
}
if (x.operators) {
j["operators"] = x.operators;
}
if (x.supportedFlags) {
j["supportedFlags"] = x.supportedFlags;
}
}

inline void from_json(const json & j, Selinux& x) {
x.enabled = get_stack_optional<bool>(j, "enabled");
}

inline void to_json(json & j, const Selinux & x) {
j = json::object();
if (x.enabled) {
j["enabled"] = x.enabled;
}
}

inline void from_json(const json & j, Linux& x) {
x.apparmor = get_stack_optional<Apparmor>(j, "apparmor");
x.capabilities = get_stack_optional<std::vector<std::string>>(j, "capabilities");
x.cgroup = get_stack_optional<Cgroup>(j, "cgroup");
x.intelRdt = get_stack_optional<IntelRdt>(j, "intelRdt");
x.namespaces = get_stack_optional<std::vector<NamespaceType>>(j, "namespaces");
x.seccomp = get_stack_optional<Seccomp>(j, "seccomp");
x.selinux = get_stack_optional<Selinux>(j, "selinux");
}

inline void to_json(json & j, const Linux & x) {
j = json::object();
if (x.apparmor) {
j["apparmor"] = x.apparmor;
}
if (x.capabilities) {
j["capabilities"] = x.capabilities;
}
if (x.cgroup) {
j["cgroup"] = x.cgroup;
}
if (x.intelRdt) {
j["intelRdt"] = x.intelRdt;
}
if (x.namespaces) {
j["namespaces"] = x.namespaces;
}
if (x.seccomp) {
j["seccomp"] = x.seccomp;
}
if (x.selinux) {
j["selinux"] = x.selinux;
}
}

inline void from_json(const json & j, Features& x) {
x.annotations = get_stack_optional<std::map<std::string, nlohmann::json>>(j, "annotations");
x.hooks = get_stack_optional<std::vector<std::string>>(j, "hooks");
x.linux_ = get_stack_optional<Linux>(j, "linux");
x.mountOptions = get_stack_optional<std::vector<std::string>>(j, "mountOptions");
x.ociVersionMax = j.at("ociVersionMax").get<std::string>();
x.ociVersionMin = j.at("ociVersionMin").get<std::string>();
}

inline void to_json(json & j, const Features & x) {
j = json::object();
if (x.annotations) {
j["annotations"] = x.annotations;
}
if (x.hooks) {
j["hooks"] = x.hooks;
}
if (x.linux_) {
j["linux"] = x.linux_;
}
if (x.mountOptions) {
j["mountOptions"] = x.mountOptions;
}
j["ociVersionMax"] = x.ociVersionMax;
j["ociVersionMin"] = x.ociVersionMin;
}

inline void from_json(const json & j, NamespaceType & x) {
if (j == "cgroup") x = NamespaceType::Cgroup;
else if (j == "ipc") x = NamespaceType::Ipc;
else if (j == "mount") x = NamespaceType::Mount;
else if (j == "network") x = NamespaceType::Network;
else if (j == "pid") x = NamespaceType::Pid;
else if (j == "time") x = NamespaceType::Time;
else if (j == "user") x = NamespaceType::User;
else if (j == "uts") x = NamespaceType::Uts;
else { throw std::runtime_error("Input JSON does not conform to schema!"); }
}

inline void to_json(json & j, const NamespaceType & x) {
switch (x) {
case NamespaceType::Cgroup: j = "cgroup"; break;
case NamespaceType::Ipc: j = "ipc"; break;
case NamespaceType::Mount: j = "mount"; break;
case NamespaceType::Network: j = "network"; break;
case NamespaceType::Pid: j = "pid"; break;
case NamespaceType::Time: j = "time"; break;
case NamespaceType::User: j = "user"; break;
case NamespaceType::Uts: j = "uts"; break;
default: throw std::runtime_error("This should not happen");
}
}

inline void from_json(const json & j, SeccompAction & x) {
if (j == "SCMP_ACT_ALLOW") x = SeccompAction::ScmpActAllow;
else if (j == "SCMP_ACT_ERRNO") x = SeccompAction::ScmpActErrno;
else if (j == "SCMP_ACT_KILL") x = SeccompAction::ScmpActKill;
else if (j == "SCMP_ACT_KILL_PROCESS") x = SeccompAction::ScmpActKillProcess;
else if (j == "SCMP_ACT_KILL_THREAD") x = SeccompAction::ScmpActKillThread;
else if (j == "SCMP_ACT_LOG") x = SeccompAction::ScmpActLog;
else if (j == "SCMP_ACT_NOTIFY") x = SeccompAction::ScmpActNotify;
else if (j == "SCMP_ACT_TRACE") x = SeccompAction::ScmpActTrace;
else if (j == "SCMP_ACT_TRAP") x = SeccompAction::ScmpActTrap;
else { throw std::runtime_error("Input JSON does not conform to schema!"); }
}

inline void to_json(json & j, const SeccompAction & x) {
switch (x) {
case SeccompAction::ScmpActAllow: j = "SCMP_ACT_ALLOW"; break;
case SeccompAction::ScmpActErrno: j = "SCMP_ACT_ERRNO"; break;
case SeccompAction::ScmpActKill: j = "SCMP_ACT_KILL"; break;
case SeccompAction::ScmpActKillProcess: j = "SCMP_ACT_KILL_PROCESS"; break;
case SeccompAction::ScmpActKillThread: j = "SCMP_ACT_KILL_THREAD"; break;
case SeccompAction::ScmpActLog: j = "SCMP_ACT_LOG"; break;
case SeccompAction::ScmpActNotify: j = "SCMP_ACT_NOTIFY"; break;
case SeccompAction::ScmpActTrace: j = "SCMP_ACT_TRACE"; break;
case SeccompAction::ScmpActTrap: j = "SCMP_ACT_TRAP"; break;
default: throw std::runtime_error("This should not happen");
}
}

inline void from_json(const json & j, SeccompArch & x) {
static std::unordered_map<std::string, SeccompArch> enumValues {
{"SCMP_ARCH_AARCH64", SeccompArch::ScmpArchAarch64},
{"SCMP_ARCH_ARM", SeccompArch::ScmpArchArm},
{"SCMP_ARCH_MIPS", SeccompArch::ScmpArchMIPS},
{"SCMP_ARCH_MIPS64", SeccompArch::ScmpArchMips64},
{"SCMP_ARCH_MIPS64N32", SeccompArch::ScmpArchMips64N32},
{"SCMP_ARCH_MIPSEL", SeccompArch::ScmpArchMipsel},
{"SCMP_ARCH_MIPSEL64", SeccompArch::ScmpArchMipsel64},
{"SCMP_ARCH_MIPSEL64N32", SeccompArch::ScmpArchMipsel64N32},
{"SCMP_ARCH_PPC", SeccompArch::ScmpArchPPC},
{"SCMP_ARCH_PARISC", SeccompArch::ScmpArchParisc},
{"SCMP_ARCH_PARISC64", SeccompArch::ScmpArchParisc64},
{"SCMP_ARCH_PPC64", SeccompArch::ScmpArchPpc64},
{"SCMP_ARCH_PPC64LE", SeccompArch::ScmpArchPpc64LE},
{"SCMP_ARCH_RISCV64", SeccompArch::ScmpArchRiscv64},
{"SCMP_ARCH_S390", SeccompArch::ScmpArchS390},
{"SCMP_ARCH_S390X", SeccompArch::ScmpArchS390X},
{"SCMP_ARCH_X32", SeccompArch::ScmpArchX32},
{"SCMP_ARCH_X86", SeccompArch::ScmpArchX86},
{"SCMP_ARCH_X86_64", SeccompArch::ScmpArchX8664},
};
auto iter = enumValues.find(j.get<std::string>());
if (iter != enumValues.end()) {
x = iter->second;
}
}

inline void to_json(json & j, const SeccompArch & x) {
switch (x) {
case SeccompArch::ScmpArchAarch64: j = "SCMP_ARCH_AARCH64"; break;
case SeccompArch::ScmpArchArm: j = "SCMP_ARCH_ARM"; break;
case SeccompArch::ScmpArchMIPS: j = "SCMP_ARCH_MIPS"; break;
case SeccompArch::ScmpArchMips64: j = "SCMP_ARCH_MIPS64"; break;
case SeccompArch::ScmpArchMips64N32: j = "SCMP_ARCH_MIPS64N32"; break;
case SeccompArch::ScmpArchMipsel: j = "SCMP_ARCH_MIPSEL"; break;
case SeccompArch::ScmpArchMipsel64: j = "SCMP_ARCH_MIPSEL64"; break;
case SeccompArch::ScmpArchMipsel64N32: j = "SCMP_ARCH_MIPSEL64N32"; break;
case SeccompArch::ScmpArchPPC: j = "SCMP_ARCH_PPC"; break;
case SeccompArch::ScmpArchParisc: j = "SCMP_ARCH_PARISC"; break;
case SeccompArch::ScmpArchParisc64: j = "SCMP_ARCH_PARISC64"; break;
case SeccompArch::ScmpArchPpc64: j = "SCMP_ARCH_PPC64"; break;
case SeccompArch::ScmpArchPpc64LE: j = "SCMP_ARCH_PPC64LE"; break;
case SeccompArch::ScmpArchRiscv64: j = "SCMP_ARCH_RISCV64"; break;
case SeccompArch::ScmpArchS390: j = "SCMP_ARCH_S390"; break;
case SeccompArch::ScmpArchS390X: j = "SCMP_ARCH_S390X"; break;
case SeccompArch::ScmpArchX32: j = "SCMP_ARCH_X32"; break;
case SeccompArch::ScmpArchX86: j = "SCMP_ARCH_X86"; break;
case SeccompArch::ScmpArchX8664: j = "SCMP_ARCH_X86_64"; break;
default: throw std::runtime_error("This should not happen");
}
}

inline void from_json(const json & j, SeccompFlag & x) {
if (j == "SECCOMP_FILTER_FLAG_LOG") x = SeccompFlag::SeccompFilterFlagLog;
else if (j == "SECCOMP_FILTER_FLAG_SPEC_ALLOW") x = SeccompFlag::SeccompFilterFlagSpecAllow;
else if (j == "SECCOMP_FILTER_FLAG_TSYNC") x = SeccompFlag::SeccompFilterFlagTsync;
else if (j == "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV") x = SeccompFlag::SeccompFilterFlagWaitKillableRecv;
else { throw std::runtime_error("Input JSON does not conform to schema!"); }
}

inline void to_json(json & j, const SeccompFlag & x) {
switch (x) {
case SeccompFlag::SeccompFilterFlagLog: j = "SECCOMP_FILTER_FLAG_LOG"; break;
case SeccompFlag::SeccompFilterFlagSpecAllow: j = "SECCOMP_FILTER_FLAG_SPEC_ALLOW"; break;
case SeccompFlag::SeccompFilterFlagTsync: j = "SECCOMP_FILTER_FLAG_TSYNC"; break;
case SeccompFlag::SeccompFilterFlagWaitKillableRecv: j = "SECCOMP_FILTER_FLAG_WAIT_KILLABLE_RECV"; break;
default: throw std::runtime_error("This should not happen");
}
}

inline void from_json(const json & j, SeccompOperators & x) {
if (j == "SCMP_CMP_EQ") x = SeccompOperators::ScmpCmpEq;
else if (j == "SCMP_CMP_GE") x = SeccompOperators::ScmpCmpGe;
else if (j == "SCMP_CMP_GT") x = SeccompOperators::ScmpCmpGt;
else if (j == "SCMP_CMP_LE") x = SeccompOperators::ScmpCmpLE;
else if (j == "SCMP_CMP_LT") x = SeccompOperators::ScmpCmpLt;
else if (j == "SCMP_CMP_MASKED_EQ") x = SeccompOperators::ScmpCmpMaskedEq;
else if (j == "SCMP_CMP_NE") x = SeccompOperators::ScmpCmpNe;
else { throw std::runtime_error("Input JSON does not conform to schema!"); }
}

inline void to_json(json & j, const SeccompOperators & x) {
switch (x) {
case SeccompOperators::ScmpCmpEq: j = "SCMP_CMP_EQ"; break;
case SeccompOperators::ScmpCmpGe: j = "SCMP_CMP_GE"; break;
case SeccompOperators::ScmpCmpGt: j = "SCMP_CMP_GT"; break;
case SeccompOperators::ScmpCmpLE: j = "SCMP_CMP_LE"; break;
case SeccompOperators::ScmpCmpLt: j = "SCMP_CMP_LT"; break;
case SeccompOperators::ScmpCmpMaskedEq: j = "SCMP_CMP_MASKED_EQ"; break;
case SeccompOperators::ScmpCmpNe: j = "SCMP_CMP_NE"; break;
default: throw std::runtime_error("This should not happen");
}
}
}
}
}
}

// clang-format on
