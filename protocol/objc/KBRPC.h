#import "KBRObject.h"
#import "KBRRequest.h"
#import "KBRRequestParams.h"

@interface KBRStringKVPair : KBRObject
@property NSString *key;
@property NSString *value;
@end

@interface KBRStatus : KBRObject
@property NSInteger code;
@property NSString *name;
@property NSString *desc;
@property NSArray *fields; /*of KBRStringKVPair*/
@end

@interface KBRUID : NSData
@end

@interface KBRLoadUserArg : KBRObject
@property KBRUID *uid;
@property NSString *username;
@property BOOL self;
@end

@interface KBRFOKID : KBRObject
@property NSData *pgpFingerprint;
@property NSData *kid;
@end

@interface KBRText : KBRObject
@property NSString *data;
@property BOOL markup;
@end

@interface KBRPgpIdentity : KBRObject
@property NSString *username;
@property NSString *comment;
@property NSString *email;
@end

@interface KBRImage : KBRObject
@property NSString *url;
@property NSInteger width;
@property NSInteger height;
@end

@interface KBRUser : KBRObject
@property KBRUID *uid;
@property NSString *username;
@property KBRImage *image;
@end

@interface KBRDevice : KBRObject
@property NSString *type;
@property NSString *name;
@property NSString *deviceID;
@end

@interface KBRSIGID : NSData
@end

@interface KBRBlockRequest : KBRRequest
- (void)announceSessionWithSid:(NSString *)sid completion:(void (^)(NSError *error))completion;

- (void)getWithBlockid:(NSData *)blockid uid:(KBRUID *)uid completion:(void (^)(NSError *error, NSData * bytes))completion;

- (void)deleteWithBlockid:(NSData *)blockid uid:(KBRUID *)uid completion:(void (^)(NSError *error))completion;

- (void)putWithBlockid:(NSData *)blockid uid:(KBRUID *)uid buf:(NSData *)buf completion:(void (^)(NSError *error))completion;

@end

@interface KBRGetCurrentStatusRes : KBRObject
@property BOOL configured;
@property BOOL registered;
@property BOOL loggedIn;
@property KBRUser *user;
@property NSString *serverUri;
@end

@interface KBRConfigRequest : KBRRequest
- (void)getCurrentStatus:(void (^)(NSError *error, KBRGetCurrentStatusRes * getCurrentStatusRes))completion;

@end

@interface KBRDeviceRequest : KBRRequest
- (void)deviceListWithSessionID:(NSInteger )sessionID completion:(void (^)(NSError *error, NSArray * items))completion;

@end

typedef NS_ENUM (NSInteger, KBRDeviceSignerKind) {
	KBRDeviceSignerKindDevice,
	KBRDeviceSignerKindPgp,
};
typedef NS_ENUM (NSInteger, KBRSelectSignerAction) {
	KBRSelectSignerActionSign,
	KBRSelectSignerActionLogout,
	KBRSelectSignerActionResetAccount,
};
@interface KBRDeviceSigner : KBRObject
@property KBRDeviceSignerKind kind;
@property NSString *deviceID;
@end

@interface KBRSelectSignerRes : KBRObject
@property KBRSelectSignerAction action;
@property KBRDeviceSigner *signer;
@end

@interface KBRDoctorUiRequest : KBRRequest
- (void)promptDeviceNameWithSessionID:(NSInteger )sessionID completion:(void (^)(NSError *error, NSString * str))completion;

- (void)selectSignerWithSessionID:(NSInteger )sessionID devices:(NSArray *)devices hasPGP:(BOOL )hasPGP completion:(void (^)(NSError *error, KBRSelectSignerRes * selectSignerRes))completion;

- (void)displaySecretWordsWithSessionID:(NSInteger )sessionID secret:(NSString *)secret xDevDescription:(NSString *)xDevDescription completion:(void (^)(NSError *error))completion;

@end

@interface KBRGpgRequest : KBRRequest
- (void)addGpgKey:(void (^)(NSError *error))completion;

@end

@interface KBRGPGKey : KBRObject
@property NSString *algorithm;
@property NSString *keyID;
@property NSString *creation;
@property NSString *expiration;
@property NSArray *identities; /*of string*/
@end

@interface KBRSelectKeyRes : KBRObject
@property NSString *keyID;
@property BOOL doSecretPush;
@end

@interface KBRGpgUiRequest : KBRRequest
- (void)wantToAddGPGKeyWithSessionID:(NSInteger )sessionID completion:(void (^)(NSError *error, BOOL  b))completion;

- (void)selectKeyAndPushOptionWithSessionID:(NSInteger )sessionID keys:(NSArray *)keys completion:(void (^)(NSError *error, KBRSelectKeyRes * selectKeyRes))completion;

- (void)selectKeyWithSessionID:(NSInteger )sessionID keys:(NSArray *)keys completion:(void (^)(NSError *error, NSString * str))completion;

@end

typedef NS_ENUM (NSInteger, KBRTrackDiffType) {
	KBRTrackDiffTypeNone,
	KBRTrackDiffTypeError,
	KBRTrackDiffTypeClash,
	KBRTrackDiffTypeDeleted,
	KBRTrackDiffTypeUpgraded,
	KBRTrackDiffTypeNew,
	KBRTrackDiffTypeRemoteFail,
	KBRTrackDiffTypeRemoteWorking,
	KBRTrackDiffTypeRemoteChanged,
};
@interface KBRTrackDiff : KBRObject
@property KBRTrackDiffType type;
@property NSString *displayMarkup;
@end

@interface KBRTrackSummary : KBRObject
@property NSInteger time;
@property BOOL isRemote;
@end

@interface KBRIdentifyOutcome : KBRObject
@property KBRStatus *status;
@property NSArray *warnings; /*of string*/
@property KBRTrackSummary *trackUsed;
@property NSInteger numTrackFailures;
@property NSInteger numTrackChanges;
@property NSInteger numProofFailures;
@property NSInteger numDeleted;
@property NSInteger numProofSuccesses;
@property NSArray *deleted; /*of KBRTrackDiff*/
@end

@interface KBRIdentifyRes : KBRObject
@property KBRUser *user;
@property KBRIdentifyOutcome *outcome;
@end

@interface KBRIdentifyRequest : KBRRequest
- (void)identifyWithSessionID:(NSInteger )sessionID uid:(KBRUID *)uid username:(NSString *)username trackStatement:(BOOL )trackStatement luba:(BOOL )luba loadSelf:(BOOL )loadSelf completion:(void (^)(NSError *error, KBRIdentifyRes * identifyRes))completion;

- (void)identifyDefaultWithSessionID:(NSInteger )sessionID username:(NSString *)username completion:(void (^)(NSError *error, KBRIdentifyRes * identifyRes))completion;

@end

@interface KBRProofStatus : KBRObject
@property NSInteger state;
@property NSInteger status;
@property NSString *desc;
@end

@interface KBRRemoteProof : KBRObject
@property NSInteger proofType;
@property NSString *key;
@property NSString *value;
@property NSString *displayMarkup;
@property KBRSIGID *sigId;
@property NSInteger mtime;
@end

@interface KBRIdentifyRow : KBRObject
@property NSInteger rowId;
@property KBRRemoteProof *proof;
@property KBRTrackDiff *trackDiff;
@end

@interface KBRIdentifyKey : KBRObject
@property NSData *pgpFingerprint;
@property NSData *KID;
@property KBRTrackDiff *trackDiff;
@end

@interface KBRCryptocurrency : KBRObject
@property NSInteger rowId;
@property NSData *pkhash;
@property NSString *address;
@end

@interface KBRIdentity : KBRObject
@property KBRStatus *status;
@property NSInteger whenLastTracked;
@property KBRIdentifyKey *key;
@property NSArray *proofs; /*of KBRIdentifyRow*/
@property NSArray *cryptocurrency; /*of KBRCryptocurrency*/
@property NSArray *deleted; /*of KBRTrackDiff*/
@end

@interface KBRSigHint : KBRObject
@property NSString *remoteId;
@property NSString *humanUrl;
@property NSString *apiUrl;
@property NSString *checkText;
@end

@interface KBRCheckResult : KBRObject
@property KBRProofStatus *proofStatus;
@property NSInteger timestamp;
@property NSString *displayMarkup;
@end

@interface KBRLinkCheckResult : KBRObject
@property NSInteger proofId;
@property KBRProofStatus *proofStatus;
@property KBRCheckResult *cached;
@property KBRTrackDiff *diff;
@property KBRTrackDiff *remoteDiff;
@property KBRSigHint *hint;
@end

@interface KBRFinishAndPromptRes : KBRObject
@property BOOL trackLocal;
@property BOOL trackRemote;
@end

@interface KBRIdentifyUiRequest : KBRRequest
- (void)finishAndPromptWithSessionID:(NSInteger )sessionID outcome:(KBRIdentifyOutcome *)outcome completion:(void (^)(NSError *error, KBRFinishAndPromptRes * finishAndPromptRes))completion;

- (void)finishWebProofCheckWithSessionID:(NSInteger )sessionID rp:(KBRRemoteProof *)rp lcr:(KBRLinkCheckResult *)lcr completion:(void (^)(NSError *error))completion;

- (void)finishSocialProofCheckWithSessionID:(NSInteger )sessionID rp:(KBRRemoteProof *)rp lcr:(KBRLinkCheckResult *)lcr completion:(void (^)(NSError *error))completion;

- (void)displayCryptocurrencyWithSessionID:(NSInteger )sessionID c:(KBRCryptocurrency *)c completion:(void (^)(NSError *error))completion;

- (void)displayKeyWithSessionID:(NSInteger )sessionID fokid:(KBRFOKID *)fokid diff:(KBRTrackDiff *)diff completion:(void (^)(NSError *error))completion;

- (void)reportLastTrackWithSessionID:(NSInteger )sessionID track:(KBRTrackSummary *)track completion:(void (^)(NSError *error))completion;

- (void)launchNetworkChecksWithSessionID:(NSInteger )sessionID id:(KBRIdentity *)id user:(KBRUser *)user completion:(void (^)(NSError *error))completion;

- (void)displayTrackStatementWithSessionID:(NSInteger )sessionID stmt:(NSString *)stmt completion:(void (^)(NSError *error))completion;

@end

typedef NS_ENUM (NSInteger, KBRLogLevel) {
	KBRLogLevelNone,
	KBRLogLevelDebug,
	KBRLogLevelInfo,
	KBRLogLevelNotice,
	KBRLogLevelWarn,
	KBRLogLevelError,
	KBRLogLevelCritical,
};
@interface KBRLogUiRequest : KBRRequest
- (void)logWithSessionID:(NSInteger )sessionID level:(KBRLogLevel )level text:(KBRText *)text completion:(void (^)(NSError *error))completion;

@end

@interface KBRLoginRequest : KBRRequest
- (void)passphraseLoginWithIdentify:(BOOL )identify username:(NSString *)username passphrase:(NSString *)passphrase completion:(void (^)(NSError *error))completion;

- (void)pubkeyLogin:(void (^)(NSError *error))completion;

- (void)logout:(void (^)(NSError *error))completion;

- (void)switchUserWithUsername:(NSString *)username completion:(void (^)(NSError *error))completion;

@end

@interface KBRLoginUiRequest : KBRRequest
- (void)getEmailOrUsernameWithSessionID:(NSInteger )sessionID completion:(void (^)(NSError *error, NSString * str))completion;

@end

@interface KBRPgpCreateUids : KBRObject
@property BOOL useDefault;
@property NSArray *ids; /*of KBRPgpIdentity*/
@end

@interface KBRMykeyRequest : KBRRequest
- (void)keyGenWithPrimaryBits:(NSInteger )primaryBits subkeyBits:(NSInteger )subkeyBits createUids:(KBRPgpCreateUids *)createUids allowMulti:(BOOL )allowMulti doExport:(BOOL )doExport completion:(void (^)(NSError *error))completion;

- (void)keyGenDefaultWithCreateUids:(KBRPgpCreateUids *)createUids pushPublic:(BOOL )pushPublic pushSecret:(BOOL )pushSecret passphrase:(NSString *)passphrase completion:(void (^)(NSError *error))completion;

- (void)deletePrimary:(void (^)(NSError *error))completion;

- (void)show:(void (^)(NSError *error))completion;

- (void)selectWithQuery:(NSString *)query allowMulti:(BOOL )allowMulti skipImport:(BOOL )skipImport completion:(void (^)(NSError *error))completion;

@end

@interface KBRProveRequest : KBRRequest
- (void)proveWithService:(NSString *)service username:(NSString *)username force:(BOOL )force completion:(void (^)(NSError *error))completion;

@end

typedef NS_ENUM (NSInteger, KBRPromptOverwriteType) {
	KBRPromptOverwriteTypeSocial,
	KBRPromptOverwriteTypeSite,
};
@interface KBRProveUiRequest : KBRRequest
- (void)promptOverwriteWithSessionID:(NSInteger )sessionID account:(NSString *)account typ:(KBRPromptOverwriteType )typ completion:(void (^)(NSError *error, BOOL  b))completion;

- (void)promptUsernameWithSessionID:(NSInteger )sessionID prompt:(NSString *)prompt prevError:(KBRStatus *)prevError completion:(void (^)(NSError *error, NSString * str))completion;

- (void)outputPrechecksWithSessionID:(NSInteger )sessionID text:(KBRText *)text completion:(void (^)(NSError *error))completion;

- (void)preProofWarningWithSessionID:(NSInteger )sessionID text:(KBRText *)text completion:(void (^)(NSError *error, BOOL  b))completion;

- (void)outputInstructionsWithSessionID:(NSInteger )sessionID instructions:(KBRText *)instructions proof:(NSString *)proof completion:(void (^)(NSError *error))completion;

- (void)okToCheckWithSessionID:(NSInteger )sessionID name:(NSString *)name attempt:(NSInteger )attempt completion:(void (^)(NSError *error, BOOL  b))completion;

- (void)displayRecheckWarningWithSessionID:(NSInteger )sessionID text:(KBRText *)text completion:(void (^)(NSError *error))completion;

@end

@interface KBRSessionToken : KBRObject
@property KBRUID *uid;
@property NSString *sid;
@property NSInteger generated;
@property NSInteger lifetime;
@end

@interface KBRQuotaRequest : KBRRequest
- (void)verifySessionWithSession:(NSString *)session completion:(void (^)(NSError *error, KBRSessionToken * sessionToken))completion;

@end

@interface KBRSecretEntryArg : KBRObject
@property NSString *desc;
@property NSString *prompt;
@property NSString *err;
@property NSString *cancel;
@property NSString *ok;
@end

@interface KBRSecretEntryRes : KBRObject
@property NSString *text;
@property BOOL canceled;
@end

@interface KBRSecretUiRequest : KBRRequest
- (void)getSecretWithSessionID:(NSInteger )sessionID pinentry:(KBRSecretEntryArg *)pinentry terminal:(KBRSecretEntryArg *)terminal completion:(void (^)(NSError *error, KBRSecretEntryRes * secretEntryRes))completion;

- (void)getNewPassphraseWithTerminalPrompt:(NSString *)terminalPrompt pinentryDesc:(NSString *)pinentryDesc pinentryPrompt:(NSString *)pinentryPrompt retryMessage:(NSString *)retryMessage completion:(void (^)(NSError *error, NSString * str))completion;

- (void)getKeybasePassphraseWithSessionID:(NSInteger )sessionID username:(NSString *)username retry:(NSString *)retry completion:(void (^)(NSError *error, NSString * str))completion;

@end

@interface KBRSession : KBRObject
@property KBRUID *uid;
@property NSString *username;
@end

@interface KBRSessionRequest : KBRRequest
- (void)currentSession:(void (^)(NSError *error, KBRSession * session))completion;

@end

@interface KBRSibkeyRequest : KBRRequest
- (void)addWithSecretPhrase:(NSString *)secretPhrase completion:(void (^)(NSError *error))completion;

@end

@interface KBRSignupRes : KBRObject
@property BOOL passphraseOk;
@property BOOL postOk;
@property BOOL writeOk;
@end

@interface KBRSignupRequest : KBRRequest
- (void)checkUsernameAvailableWithUsername:(NSString *)username completion:(void (^)(NSError *error))completion;

- (void)signupWithEmail:(NSString *)email inviteCode:(NSString *)inviteCode passphrase:(NSString *)passphrase username:(NSString *)username deviceName:(NSString *)deviceName completion:(void (^)(NSError *error, KBRSignupRes * signupRes))completion;

- (void)inviteRequestWithEmail:(NSString *)email fullname:(NSString *)fullname notes:(NSString *)notes completion:(void (^)(NSError *error))completion;

@end

@interface KBRTrackRequest : KBRRequest
- (void)trackWithSessionID:(NSInteger )sessionID theirName:(NSString *)theirName completion:(void (^)(NSError *error))completion;

@end

@interface KBRUiRequest : KBRRequest
- (void)promptYesNoWithSessionID:(NSInteger )sessionID text:(KBRText *)text def:(BOOL )def completion:(void (^)(NSError *error, BOOL  b))completion;

@end
@interface KBRAnnounceSessionRequestParams : KBRRequestParams
@property NSString *sid;
@end
@interface KBRGetRequestParams : KBRRequestParams
@property NSData *blockid;
@property KBRUID *uid;
@end
@interface KBRDeleteRequestParams : KBRRequestParams
@property NSData *blockid;
@property KBRUID *uid;
@end
@interface KBRPutRequestParams : KBRRequestParams
@property NSData *blockid;
@property KBRUID *uid;
@property NSData *buf;
@end
@interface KBRDeviceListRequestParams : KBRRequestParams
@property NSInteger sessionID;
@end
@interface KBRPromptDeviceNameRequestParams : KBRRequestParams
@property NSInteger sessionID;
@end
@interface KBRSelectSignerRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSArray *devices;
@property BOOL hasPGP;
@end
@interface KBRDisplaySecretWordsRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSString *secret;
@property NSString *xDevDescription;
@end
@interface KBRWantToAddGPGKeyRequestParams : KBRRequestParams
@property NSInteger sessionID;
@end
@interface KBRSelectKeyAndPushOptionRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSArray *keys;
@end
@interface KBRSelectKeyRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSArray *keys;
@end
@interface KBRIdentifyRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRUID *uid;
@property NSString *username;
@property BOOL trackStatement;
@property BOOL luba;
@property BOOL loadSelf;
@end
@interface KBRIdentifyDefaultRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSString *username;
@end
@interface KBRFinishAndPromptRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRIdentifyOutcome *outcome;
@end
@interface KBRFinishWebProofCheckRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRRemoteProof *rp;
@property KBRLinkCheckResult *lcr;
@end
@interface KBRFinishSocialProofCheckRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRRemoteProof *rp;
@property KBRLinkCheckResult *lcr;
@end
@interface KBRDisplayCryptocurrencyRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRCryptocurrency *c;
@end
@interface KBRDisplayKeyRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRFOKID *fokid;
@property KBRTrackDiff *diff;
@end
@interface KBRReportLastTrackRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRTrackSummary *track;
@end
@interface KBRLaunchNetworkChecksRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRIdentity *id;
@property KBRUser *user;
@end
@interface KBRDisplayTrackStatementRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSString *stmt;
@end
@interface KBRLogRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRLogLevel level;
@property KBRText *text;
@end
@interface KBRPassphraseLoginRequestParams : KBRRequestParams
@property BOOL identify;
@property NSString *username;
@property NSString *passphrase;
@end
@interface KBRSwitchUserRequestParams : KBRRequestParams
@property NSString *username;
@end
@interface KBRGetEmailOrUsernameRequestParams : KBRRequestParams
@property NSInteger sessionID;
@end
@interface KBRKeyGenRequestParams : KBRRequestParams
@property NSInteger primaryBits;
@property NSInteger subkeyBits;
@property KBRPgpCreateUids *createUids;
@property BOOL allowMulti;
@property BOOL doExport;
@end
@interface KBRKeyGenDefaultRequestParams : KBRRequestParams
@property KBRPgpCreateUids *createUids;
@property BOOL pushPublic;
@property BOOL pushSecret;
@property NSString *passphrase;
@end
@interface KBRSelectRequestParams : KBRRequestParams
@property NSString *query;
@property BOOL allowMulti;
@property BOOL skipImport;
@end
@interface KBRProveRequestParams : KBRRequestParams
@property NSString *service;
@property NSString *username;
@property BOOL force;
@end
@interface KBRPromptOverwriteRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSString *account;
@property KBRPromptOverwriteType typ;
@end
@interface KBRPromptUsernameRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSString *prompt;
@property KBRStatus *prevError;
@end
@interface KBROutputPrechecksRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRText *text;
@end
@interface KBRPreProofWarningRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRText *text;
@end
@interface KBROutputInstructionsRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRText *instructions;
@property NSString *proof;
@end
@interface KBROkToCheckRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSString *name;
@property NSInteger attempt;
@end
@interface KBRDisplayRecheckWarningRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRText *text;
@end
@interface KBRVerifySessionRequestParams : KBRRequestParams
@property NSString *session;
@end
@interface KBRGetSecretRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRSecretEntryArg *pinentry;
@property KBRSecretEntryArg *terminal;
@end
@interface KBRGetNewPassphraseRequestParams : KBRRequestParams
@property NSString *terminalPrompt;
@property NSString *pinentryDesc;
@property NSString *pinentryPrompt;
@property NSString *retryMessage;
@end
@interface KBRGetKeybasePassphraseRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSString *username;
@property NSString *retry;
@end
@interface KBRAddRequestParams : KBRRequestParams
@property NSString *secretPhrase;
@end
@interface KBRCheckUsernameAvailableRequestParams : KBRRequestParams
@property NSString *username;
@end
@interface KBRSignupRequestParams : KBRRequestParams
@property NSString *email;
@property NSString *inviteCode;
@property NSString *passphrase;
@property NSString *username;
@property NSString *deviceName;
@end
@interface KBRInviteRequestRequestParams : KBRRequestParams
@property NSString *email;
@property NSString *fullname;
@property NSString *notes;
@end
@interface KBRTrackRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property NSString *theirName;
@end
@interface KBRPromptYesNoRequestParams : KBRRequestParams
@property NSInteger sessionID;
@property KBRText *text;
@property BOOL def;
@end