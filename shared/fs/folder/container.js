// @flow
import * as I from 'immutable'
import {namedConnect} from '../../util/container'
import Folder from '.'
import * as Types from '../../constants/types/fs'
import * as Constants from '../../constants/fs'
import * as FsGen from '../../actions/fs-gen'
import flags from '../../util/feature-flags'

const mapStateToProps = (state, {path}) => ({
  _pathItem: state.fs.pathItems.get(path, Constants.unknownPathItem),
  _username: state.config.username,
  offline: !state.fs.kbfsDaemonStatus.online,
  resetBannerType: Constants.resetBannerType(state, path),
  shouldShowSFMIBanner: state.fs.sfmi.showingBanner,
})

const mapDispatchToProps = (dispatch, {path}: OwnProps) => ({
  onAttach: (paths: Array<string>) => {
    paths.forEach(localPath => dispatch(FsGen.createUpload({localPath, parentPath: path})))
  },
})

const mergeProps = (stateProps, dispatchProps, {path, routePath}) => ({
  offline:
    flags.kbfsOfflineMode &&
    stateProps.offline &&
    Types.getPathLevel(path) > 2 &&
    stateProps._pathItem.prefetchStatus !== Constants.prefetchComplete,
  onAttach: stateProps._pathItem.writable ? dispatchProps.onAttach : null,
  path,
  resetBannerType: stateProps.resetBannerType,
  routePath,
  shouldShowSFMIBanner: stateProps.shouldShowSFMIBanner,
})

type OwnProps = {|
  path: Types.Path,
  routePath: I.List<string>,
|}

// flow can't figure out type when compose is used.
export default namedConnect<OwnProps, _, _, _, _>(mapStateToProps, mapDispatchToProps, mergeProps, 'Folder')(
  Folder
)
